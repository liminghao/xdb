all:
	g++ data_server.cc server_main.cc xdb_server.cc -o xdb_server -g -L ../deps/muduo/build/debug-install/lib -I ../deps/muduo/build/debug-install/include -lmuduo_base -lmuduo_net -lpthread

clean:
	rm -f xdb_server
