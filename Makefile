all:
	g++ ./deps/tinyxml/*.cpp src/*.cc src/protocol/*.cc src/util/*.cc -o xdb_server -g -L ./deps/muduo/build/debug-install/lib -I ./deps/muduo/build/debug-install/include -I src/protocol -I src/util -lmuduo_base -lmuduo_net -lpthread -D__STDC_FORMAT_MACROS -Wno-deprecated -I deps/leveldb_v1.18/leveldb/include deps/leveldb_v1.18/leveldb/libleveldb.a -I ./deps/tinyxml

clean:
	rm -f xdb_server
	rm -rf xdb_data
