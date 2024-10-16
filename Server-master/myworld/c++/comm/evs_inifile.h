#ifndef _EVS_INI_FILE_
#define _EVS_INI_FILE_

namespace evslib
{
	class CIniFile
	{
	public:

		/**
		@brief Load ini file into buffer.
		@param sIniFile: Ini file name.
		*/
		CIniFile(const char *szIniFile);

		/**
		@brief Release buffer.
		*/
		~CIniFile();

		/**
		@brief Read the value of specific item and fill it into string buffer.
		@param sSection: Ini file section name.
		@param sItem: Ini file item name.
		@param sDefault: The default value. When the function fail to locate the item, it will
		fill the default value into sValue and return 1.
		@param sValue: The buffer to store value.
		@param nValueLen: The length of the sValue buffer.
		@return 0=OK, 1=FAIL
		*/
		int GetString(const char *sSection, const char *sItem,
			const char *sDefault, char *sValue, const int nValueLen);

		/**
		@param sSection: Ini file section name.
		@param sItem: Ini file item name.
		@param nDefault: The default value. When the function fail to locate the item, it will
		fill the default value into sValue and return 1.
		@param nValue: The buffer to store value.
		@brief Read the value of specific item and fill it into integer buffer.
		@return 0=OK, 1=FAIL
		*/
		int GetInt(const char *sSection, const char *sItem, const int nDefault,
			int *nValue);
		int GetInt(const char *sSection, const char *sItem, const int nDefault,
			unsigned int *nValue);
		int GetInt(const char *sSection, const char *sItem, const size_t nDefault,size_t *nValue);

		int GetULongLong(const char *szSection, const char *szItem, unsigned long long ullDefault,
			unsigned long long *pullValue);

		/**
		@brief To see if the cpIni class successfully load ini file into buffer.
		@return 1=OK, 0=FAIL
		*/
		int IsValid();

	private:

		/// Store Ini file text buffer
		char *m_szBuffer;
		int m_iSize;
	};
} /* evslib */

#endif
