[centerd]
    server.bind     = ##IP##:8899
    server.max-fds  = 10000
    server.max-read-idle = 30
    
    server.log.cfg         = ../cfg/tlog.xml
	server.file.max-trans-bytes	= 65535
	
	server.location			= ##IP##
	server.master			= 1

[bus]
	bus.key			= 8899
	bus.size		= 1024000
	bus.bind		= 0.0.0.1

[cascade]
	cascade.conn			= ##IP##:8900