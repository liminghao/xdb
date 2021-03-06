all:
	cd tools && ./protoc -I../src --cpp_out=../src/generate_code ../src/binlog/pb_binlog.proto && cd -

	g++ src/generate_code/binlog/*.cc ./deps/tinyxml/*.cpp src/*.cc src/protocol/*.cc src/util/*.cc src/binlog/*.cc -o xdb_server -g -L ./deps/muduo/build/debug-install/lib -I ./src/generate_code -I ./deps/muduo/build/debug-install/include -I src/protocol -I src/binlog -I src/util -lmuduo_base -lmuduo_net -lpthread -D__STDC_FORMAT_MACROS -Wno-deprecated -I deps/leveldb_v1.18/leveldb/include deps/leveldb_v1.18/leveldb/libleveldb.a -I ./deps/tinyxml -I deps/protobuf/output/include/ -L deps/protobuf/output/lib -lprotobuf

clean:
	rm -rf src/generate_code/*
	rm -f xdb_server
	rm -rf xdb_data
