using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace SVNAuthHandler
{
	class Program
	{
		private static string extProgram = "svnlook.exe";
		//TODO HERE 改为读取配置
		private static string WorkPath = "f:\\update_resdb\\";

		private static string InvokeProc(string reposPath, string txn, string lookType, bool bRev)
		{
			Process proc = new Process();
			string arg = " -t ";
			if (lookType == "lock")
			{
				arg = " ";
			}
			if (bRev)
			{
				arg = " -r ";
			}
			proc.StartInfo.FileName = extProgram;
			proc.StartInfo.UseShellExecute = false;
			proc.StartInfo.RedirectStandardError = true;
			proc.StartInfo.RedirectStandardOutput = true;
			proc.StartInfo.CreateNoWindow = true;
			proc.StartInfo.Arguments = lookType + " " + reposPath + arg + txn;
			proc.Start();
			string stdout = proc.StandardOutput.ReadToEnd();
			proc.WaitForExit();
			return stdout;
		}

		static bool CommitCheck(string reposPath, string txn)
		{
			string user = InvokeProc(reposPath, txn, "author", false);
			if (user.Contains("taguser"))
			{
				return true;
			}
			string paths = InvokeProc(reposPath, txn, "changed", false);
			string[] res = paths.Split("\r\n".ToCharArray());
			string[] files = new string[res.Length / 2];
			if (files.Length == 0)
			{
				return false;
			}
			for (int i = 0; i < files.Length; i++)
			{
				files[i] = res[i * 2].Remove(0, 4);
			}
			for (int i = 0; i < files.Length; i++)
			{
				string path = files[i];
				if (path.Contains("/resdb_snapshot/"))
				{
					string log = InvokeProc(reposPath, txn, "log", false);
					if (log.Length < 5)
					{
						Console.Error.WriteLine("请输入修改日志！");
						return false;
					}
					//删除和添加不需要lock
					if (!(res[i * 2][0] == 'A' || res[i * 2][0] == 'D'))
					{
						string author = InvokeProc(reposPath, txn, "author", false);
						author = author.TrimEnd('\r', '\n');
						string Lock = InvokeProc(reposPath, path, "lock", false);
						Console.Error.Write(Lock);
						if (Lock.Length == 0)
						{
							Console.Error.WriteLine("请lock后，再提交！");
							return false;
						}
						string[] lines = Lock.Split("\r\n".ToCharArray());
						for (int j = 0; j < lines.Length; j++)
						{
							string line = lines[j];
							line = line.Trim();
							if (line.Contains("Owner:"))
							{
								string owner = line.Remove(0, 7);
								if (author != owner)
								{
									Console.Error.WriteLine("请lock后，再提交！");
									return false;
								}
								break;
							}
						}
					}
				}
			}
			return true;
		}

		private static bool CommitPost( string reposPath, string txn, string versionFlag )
		{
			string user = InvokeProc(reposPath, txn, "author", true);
			if (user.Contains("taguser"))
			{
				return true;
			}
			string paths = InvokeProc(reposPath, txn, "changed", true);
			string[] res = paths.Split("\r\n".ToCharArray());
			string[] files = new string[res.Length / 2];
			if (files.Length == 0)
			{
				files = new string[1];
			}
			for (int i = 0; i < files.Length; i++)
			{
				files[i] = res[i * 2].Remove(0, 4);
			}
			bool bUpdate = false;
			for (int i = 0; i < files.Length; i++)
			{
				string path = files[i];
				if (res[i * 2][0] == 'D')
				{
					continue;
				}

				if (path.Contains("/resdb_snapshot/"))
				{
					string tableName = System.IO.Path.GetFileNameWithoutExtension(path);
					string directory = System.IO.Path.GetDirectoryName(path);

					Process proc = new Process();
					proc.StartInfo.UseShellExecute = false;
					proc.StartInfo.RedirectStandardError = false;
					proc.StartInfo.RedirectStandardOutput = false;
					proc.StartInfo.CreateNoWindow = true;

					if (!bUpdate)
					{
                        if ( !(versionFlag.Contains("ngame") || versionFlag.Contains("vgame")) )
                        {
                            proc.StartInfo.FileName = @"svn.exe";
						    proc.StartInfo.Arguments = "update " + WorkPath + directory;
						    proc.Start();
						    proc.WaitForExit();

                        }
                        if ( versionFlag.Contains("ngame") )
                        {
                            proc.StartInfo.FileName = @"svn.exe";
                            proc.StartInfo.Arguments = "update " + WorkPath + "ngame\\" + directory;
                            proc.Start();
                            proc.WaitForExit();
                        }
                        else if ( versionFlag.Contains("vgame") )
                        {
                            proc.StartInfo.FileName = @"svn.exe";
                            proc.StartInfo.Arguments = "update " + WorkPath + "vgame\\" + directory;
                            proc.Start();
                            proc.WaitForExit();
                        }
                        else if ( path.Contains("/deployed/"))
						{
							proc.StartInfo.Arguments = "update " + WorkPath + "deployed\\CS交互协议";
							proc.Start();
							proc.WaitForExit();
						}
						else
						{
							proc.StartInfo.Arguments = "update " + WorkPath + "trunk\\CS交互协议";
							proc.Start();
							proc.WaitForExit();

						}
						bUpdate = true;
					}
					//如果是以 .csv 结尾的，
					if (path.EndsWith(".csv"))
					{
                        if( versionFlag.Contains("ngame") )
                        {
                            proc.StartInfo.FileName = WorkPath + "ngame\\" + directory + "\\resdb_snapshot.exe";
                            proc.StartInfo.WorkingDirectory = WorkPath + "ngame\\" + directory;
                            proc.StartInfo.Arguments = tableName + " " + WorkPath + "ngame\\" + path;

                            proc.Start();
                            proc.WaitForExit();
                            if (proc.ExitCode == -1)
                            {
                                Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
                                return false;
                            }
                        }
                        else if( versionFlag.Contains("vgame") )
                        {
                            proc.StartInfo.FileName = WorkPath + "vgame\\" + directory + "\\resdb_snapshot.exe";
                            proc.StartInfo.WorkingDirectory = WorkPath + "vgame\\" + directory;
                            proc.StartInfo.Arguments = tableName + " " + WorkPath + "vgame\\" + path;

                            proc.Start();
                            proc.WaitForExit();
                            if (proc.ExitCode == -1)
                            {
                                Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
                                return false;
                            }
                        }
						else if (path.ToLower().Contains("trunk/resdb_snapshot/resdb_cehua_test"))
						{
							proc.StartInfo.FileName = WorkPath + directory + "\\resdb_snapshot.exe";
							proc.StartInfo.WorkingDirectory = WorkPath + directory;
							proc.StartInfo.Arguments = tableName + " " + WorkPath + path;

							proc.Start();
							proc.WaitForExit();
							if (proc.ExitCode == -1)
							{
								Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
								return false;
							}
						}
						else if (path.ToLower().Contains("deployed/resdb_snapshot/resdb_deploy"))
						{
							proc.StartInfo.FileName = WorkPath + directory + "\\resdb_snapshot.exe";
							proc.StartInfo.WorkingDirectory = WorkPath + directory;
							proc.StartInfo.Arguments = tableName + " " + WorkPath + path;

							proc.Start();
							proc.WaitForExit();
							if (proc.ExitCode == -1)
							{
								Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
								return false;
							}
						}
						else if (path.ToLower().Contains("trunk/resdb_snapshot/resdb"))
						{
							proc.StartInfo.FileName = WorkPath + directory + "\\resdb_snapshot.exe";
							proc.StartInfo.WorkingDirectory = WorkPath + directory;
							proc.StartInfo.Arguments = tableName + " " + WorkPath + path;

							proc.Start();
							proc.WaitForExit();
							if( proc.ExitCode == -1 )
							{
								Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
								return false;
							}
						}
						else if (path.ToLower().Contains("wai/resdb_snapshot/resdb"))
						{
							proc.StartInfo.FileName = WorkPath + directory + "\\resdb_snapshot.exe";
							proc.StartInfo.WorkingDirectory = WorkPath + directory;
							proc.StartInfo.Arguments = tableName + " " + WorkPath + path;

							proc.Start();
							proc.WaitForExit();
							if (proc.ExitCode == -1)
							{
								Console.Error.WriteLine("文件提交成功，更新数据失败，请检查文件格式");
								return false;
							}
						}
					}
				}
			}
			return true;
		}

		static int Main(string[] args)
		{
			if (args[0] == "pre")
			{
				if (CommitCheck(args[1], args[2]))
				{
					return 0;
				}
			}
			else if (args[0] == "post")
			{
				if (CommitPost(args[1], args[2], args[3]))
				{
					return 0;
				}
			}
			return -1;
		}
	}
}
