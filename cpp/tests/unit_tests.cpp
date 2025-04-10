/*
 * Copyright 2023 PixelsDB.
 *
 * This file is part of Pixels.
 *
 * Pixels is free software: you can redistribute it and/or modify
 * it under the terms of the Affero GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Pixels is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Affero GNU General Public License for more details.
 *
 * You should have received a copy of the Affero GNU General Public
 * License along with Pixels.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

/*
 * @author liyu
 * @create 2023-03-13
 */
#include "encoding/RunLenIntEncoder.h"
#include "encoding/RunLenIntDecoder.h"

#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <string>
#include <random>
#include "PixelsBitMask.h"
using namespace std;
//
//
//TEST(reader, ByteBufferPopulateChar) {
//    // randomly generate a file
//    std::string path = "/home/yuly/project/pixels-reader-cxx/tests/data/nation_0_1.pxl";
//    // read target data
//    FILE * fp = fopen(path.c_str(), "r");
//    // checking if the file exist or not
//    if (fp == nullptr) {
//        throw std::runtime_error("File not found");
//    }
//    fseek(fp, 0L, SEEK_END);
//    long length = ftell(fp);
//    fclose(fp);
//    char * target;
//	posix_memalign((void**)&target, 4096, 4096);
//    int fd = open(path.c_str(), O_RDONLY|O_DIRECT);
//    if(pread(fd, target, 4096, 0) == -1) {
//        throw std::runtime_error("pread fail");
//    }
//    close(fd);
//    // read the data via physicalReader;
//    auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//    auto fsReader = PhysicalReaderUtil::newPhysicalReader(
//            storage, path);
//
////    std::cout<<"start testing..."<<std::endl;
////    for(long i = 0; i < length; i++) {
////        EXPECT_EQ(fsReader->readChar(), target[i]);
////    }
//}
//
//TEST(reader, PixelsVersion) {
//    EXPECT_EQ(PixelsVersion::V1, PixelsVersion::currentVersion());
//    EXPECT_EQ(PixelsVersion::V1, PixelsVersion::from(1));
//}
//
//TEST(ByteBuffer, read) {
//    srand(10);
//    int bufSize = 1000000;
//    auto * buf = new uint8_t[bufSize];
//    for(int i = 0; i < bufSize; i++) {
//        buf[i] = rand() % 256;
//    }
//
//    std::shared_ptr<ByteBuffer> bb = std::make_shared<ByteBuffer>(buf, bufSize);
//    int testNum = 100;
//    auto * target = new uint8_t[bufSize];
//    for(int i = 0; i < testNum; i++) {
//        int offset = rand() % bufSize;
//        int readPos = rand() % bufSize;
//        bb->setReadPos(readPos);
//        int length = rand() % (bufSize - std::max(offset, readPos));
//        int actualLength = bb->read(target, offset, length);
//        EXPECT_EQ(length, actualLength);
//        // check if target is the same as buffer
//        for(int j = 0; j < length; j++) {
//            EXPECT_EQ(target[offset + j], buf[readPos + j]);
//        }
//    }
//    delete[] target;
//    // don't delete buf. bb will delete it.
//}
//
//TEST(reader, recordReaderSingleTable) {
//    std::string dataset = "/home/yuly/project/duckdb/extension/pixels/pixels-reader-cxx/tests/data/nation_0_1.pxl";
//	auto footerCache = std::make_shared<PixelsFooterCache>();
//    auto builder = std::make_shared<PixelsReaderBuilder>();
//    auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//    auto pixelsReader = builder
//            ->setPath(dataset)
//            ->setStorage(storage)
//            ->setPixelsFooterCache(footerCache)
//            ->build();
//
//	PixelsReaderOption option;
//	option.setSkipCorruptRecords(true);
//	option.setTolerantSchemaEvolution(true);
//	option.setEnableEncodedColumnVector(true);
//
//	// includeCols comes from the caller of PixelsPageSource
//	std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//	option.setIncludeCols(includeCols);
//	option.setRGRange(0, 1);
//	option.setQueryId(1);
//    auto pixelsRecordReader = pixelsReader->read(option);
//
//	std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(13, false);
//
//
//	EXPECT_FALSE(v->endOfFile);
//	EXPECT_FALSE(pixelsRecordReader->isEndOfFile());
//	EXPECT_EQ(v->rowCount, 13);
//    for(const auto& col: v->cols) {
//        std::cout<<"------"<<std::endl;
//        col->print(v->rowCount);
//    }
//    std::shared_ptr<VectorizedRowBatch> v1 = pixelsRecordReader->readBatch(120, false);
//	EXPECT_TRUE(v1->endOfFile);
//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	EXPECT_EQ(v1->rowCount, 12);
//	std::cout<<"------"<<std::endl;
//    std::cout<<"------"<<std::endl;
//    std::cout<<"------"<<std::endl;
//    std::cout<<"------"<<std::endl;
//    for(const auto& col: v1->cols) {
//        std::cout<<"------"<<std::endl;
//        col->print(v1->rowCount);
//    }
//	std::shared_ptr<VectorizedRowBatch> v2 = pixelsRecordReader->readBatch(120, false);
//	EXPECT_TRUE(v2->endOfFile);
//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	EXPECT_EQ(v2->rowCount, 0);
//
//}
//
//TEST(reader, recordReaderMultipleTable) {
//	{
//		std::string dataset = "/home/yuly/project/duckdb/extension/pixels/pixels-reader-cxx/tests/data/nation_0_1.pxl";
//		auto footerCache = std::make_shared<PixelsFooterCache>();
//		auto * builder = new PixelsReaderBuilder;
//		auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//		auto pixelsReader = builder
//		                        ->setPath(dataset)
//		                        ->setStorage(storage)
//		                        ->setPixelsFooterCache(footerCache)
//		                        ->build();
//
//		PixelsReaderOption option;
//		option.setSkipCorruptRecords(true);
//		option.setTolerantSchemaEvolution(true);
//		option.setEnableEncodedColumnVector(true);
//
//		// includeCols comes from the caller of PixelsPageSource
//		std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//		option.setIncludeCols(includeCols);
//		option.setRGRange(0, 1);
//		option.setQueryId(1);
//		auto pixelsRecordReader = pixelsReader->read(option);
//		std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(13, false);
//		EXPECT_FALSE(v->endOfFile);
//		EXPECT_FALSE(pixelsRecordReader->isEndOfFile());
//		EXPECT_EQ(v->rowCount, 13);
//		//    for(const auto& col: v->cols) {
//		//        std::cout<<"------"<<std::endl;
//		//        col->print();
//		//    }
//		std::shared_ptr<VectorizedRowBatch> v1 = pixelsRecordReader->readBatch(120, false);
//		EXPECT_TRUE(v1->endOfFile);
//		EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//		EXPECT_EQ(v1->rowCount, 12);
//		//	std::cout<<"------"<<std::endl;
//		//    std::cout<<"------"<<std::endl;
//		//    std::cout<<"------"<<std::endl;
//		//    std::cout<<"------"<<std::endl;
//		//    for(const auto& col: v1->cols) {
//		//        std::cout<<"------"<<std::endl;
//		//        col->print();
//		//    }
//		std::shared_ptr<VectorizedRowBatch> v2 = pixelsRecordReader->readBatch(120, false);
//		EXPECT_TRUE(v2->endOfFile);
//		EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//		EXPECT_EQ(v2->rowCount, 0);
//	}
//	{
//		std::string dataset = "/home/yuly/project/duckdb/extension/pixels/pixels-reader-cxx/tests/data/region_0_1.pxl";
//		auto footerCache = std::make_shared<PixelsFooterCache>();
//		auto * builder = new PixelsReaderBuilder;
//		auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//		auto pixelsReader = builder
//		                        ->setPath(dataset)
//		                        ->setStorage(storage)
//		                        ->setPixelsFooterCache(footerCache)
//		                        ->build();
//
//		PixelsReaderOption option;
//		option.setSkipCorruptRecords(true);
//		option.setTolerantSchemaEvolution(true);
//		option.setEnableEncodedColumnVector(true);
//
//		// includeCols comes from the caller of PixelsPageSource
//		std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//		option.setIncludeCols(includeCols);
//		option.setRGRange(0, 1);
//		option.setQueryId(1);
//		auto pixelsRecordReader = pixelsReader->read(option);
//		std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(13, false);
//		EXPECT_TRUE(v->endOfFile);
//		EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//		EXPECT_EQ(v->rowCount, 5);
////		    for(const auto& col: v->cols) {
////		        std::cout<<"------"<<std::endl;
////		        col->print();
////		    }
//
//	}
//
//
//}
//
//TEST(reader, testDateReader) {
//	std::string dataset = "/home/yuly/project/data/orders_0_1.pxl";
//	auto footerCache = std::make_shared<PixelsFooterCache>();
//	auto builder = std::make_shared<PixelsReaderBuilder>();
//	auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//	auto pixelsReader = builder
//	                        ->setPath(dataset)
//	                        ->setStorage(storage)
//	                        ->setPixelsFooterCache(footerCache)
//	                        ->build();
//
//	PixelsReaderOption option;
//	option.setSkipCorruptRecords(true);
//	option.setTolerantSchemaEvolution(true);
//	option.setEnableEncodedColumnVector(true);
//
//	// includeCols comes from the caller of PixelsPageSource
////	std::vector<std::string> includeCols;
////	includeCols.emplace_back("o_orderdate");
//     std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//	option.setIncludeCols(includeCols);
//	option.setRGRange(0, 1);
//	option.setQueryId(1);
//	auto pixelsRecordReader = pixelsReader->read(option);
//	std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(1000, false);
//	//	EXPECT_FALSE(v->endOfFile);
//	//	EXPECT_FALSE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v->rowCount, 13);
//	for(const auto& col: v->cols) {
//		std::cout<<"------"<<std::endl;
//		col->print(v->rowCount);
//	}
//	//    std::shared_ptr<VectorizedRowBatch> v1 = pixelsRecordReader->readBatch(120, false);
//	//	EXPECT_TRUE(v1->endOfFile);
//	//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v1->rowCount, 12);
//	//	std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    for(const auto& col: v1->cols) {
//	//        std::cout<<"------"<<std::endl;
//	//        col->print(v1->rowCount);
//	//    }
//	//	std::shared_ptr<VectorizedRowBatch> v2 = pixelsRecordReader->readBatch(120, false);
//	//	EXPECT_TRUE(v2->endOfFile);
//	//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v2->rowCount, 0);
//
//}
//
//TEST(reader, testMultipleRowGroup) {
//	std::string dataset = "/home/yuly/project/pixels-reader-cxx/tests/data/nation_multiple/v-0-order/nation_a.pxl";
//	auto footerCache = std::make_shared<PixelsFooterCache>();
//	auto builder = std::make_shared<PixelsReaderBuilder>();
//	auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//	auto pixelsReader = builder
//	                        ->setPath(dataset)
//	                        ->setStorage(storage)
//	                        ->setPixelsFooterCache(footerCache)
//	                        ->build();
//
//	PixelsReaderOption option;
//	option.setSkipCorruptRecords(true);
//	option.setTolerantSchemaEvolution(true);
//	option.setEnableEncodedColumnVector(true);
//
//	// includeCols comes from the caller of PixelsPageSource
//	std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//	option.setIncludeCols(includeCols);
//	option.setRGRange(1, 1);
//	option.setQueryId(1);
//	auto pixelsRecordReader = pixelsReader->read(option);
//	std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(10000, false);
//	//	EXPECT_FALSE(v->endOfFile);
//	//	EXPECT_FALSE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v->rowCount, 13);
//	for(const auto& col: v->cols) {
//		std::cout<<"------"<<std::endl;
//		col->print(v->rowCount);
//	}
//	//    std::shared_ptr<VectorizedRowBatch> v1 = pixelsRecordReader->readBatch(120, false);
//	//	EXPECT_TRUE(v1->endOfFile);
//	//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v1->rowCount, 12);
//	//	std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    std::cout<<"------"<<std::endl;
//	//    for(const auto& col: v1->cols) {
//	//        std::cout<<"------"<<std::endl;
//	//        col->print(v1->rowCount);
//	//    }
//	//	std::shared_ptr<VectorizedRowBatch> v2 = pixelsRecordReader->readBatch(120, false);
//	//	EXPECT_TRUE(v2->endOfFile);
//	//	EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
//	//	EXPECT_EQ(v2->rowCount, 0);
//
//}
//
//
//TEST(reader, fileTail) {
//    std::string path = "/home/liyu/pixels-reader-cxx/tests/data/20230224150144_3.pxl";
//    auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//    auto fsReader = PhysicalReaderUtil::newPhysicalReader(
//            storage, path);
//    pixels::proto::FileTail fileTail;
//
//    if(fsReader == nullptr) {
//        throw std::runtime_error("Failed to create PixelsReader due to error of creating PhysicalReader");
//    }
//
//    PixelsFooterCache pixelsFooterCache;
//    std::string filename = fsReader->getName();
//    EXPECT_EQ(filename, "20230224150144_3.pxl");
//    for(int i = 0; i < 2; i++) {
//        if(pixelsFooterCache.containsFileTail(filename)) {
//            EXPECT_EQ(i, 1);
//            fileTail = *pixelsFooterCache.getFileTail(filename);
//        } else {
//            // get FileTail
//            EXPECT_EQ(i, 0);
//            long fileLen = fsReader->getFileLength();
//            fsReader->seek(fileLen - (long)sizeof(long));
//            long fileTailOffset = fsReader->readLong();
//            int fileTailLength = (int) (fileLen - fileTailOffset - sizeof(long));
//            fsReader->seek(fileTailOffset);
//            std::shared_ptr<ByteBuffer> fileTailBuffer = fsReader->readFully(fileTailLength);
//            if(!fileTail.ParseFromArray(fileTailBuffer->getPointer(),
//                                        fileTailLength)) {
//                throw std::runtime_error("paring FileTail error!");
//            }
//            pixelsFooterCache.putFileTail(filename, std::make_shared<pixels::proto::FileTail>(fileTail));
//        }
//    }
//}
//
//TEST(reader, ConfigFactory) {
//	auto a = ConfigFactory::Instance();
//	a.Print();
//}
//
//TEST(reader, sortMergeRandomTest) {
//    srand(66);
//    int requestSize = 10000;
//    long maxLimit = 2000000000;
//    RequestBatch requests;
//    auto * scheduler = SortMergeScheduler::Instance();
//    auto * sortMergeScheduler = (SortMergeScheduler *)(scheduler);
//    long currOffset = 0;
//    std::vector<int> targetStart;
//    std::vector<int> targetLength;
//    int currStart = -1;
//    int currLength = -1;
//    int maxGap = std::stoi(ConfigFactory::Instance().getProperty("read.request.merge.gap"));
//    for(int i = 0; i < requestSize; i++) {
//        if (maxLimit - currOffset <= 1) {
//            break;
//        }
//        int start = currOffset + rand() % (maxLimit - currOffset);
//        int length = 1 + rand() % (maxLimit - start);
//        if(currStart == -1) {
//            currStart = start;
//            currLength = length;
//        }
//        currOffset = start + length;
//        Request request(0, start, length);
//        requests.add(request);
//
//        int currEnd = currStart + currLength;
//        if(start - currEnd <= maxGap) {
//            currLength += start - currEnd + length;
//        } else {
//            targetStart.emplace_back(currStart);
//            targetLength.emplace_back(currLength);
//            currStart = start;
//            currLength = length;
//        }
//    }
//    targetStart.emplace_back(currStart);
//    targetLength.emplace_back(currLength);
//    auto merged = sortMergeScheduler->sortMerge(requests, 0);
//    EXPECT_EQ(merged.size(), targetStart.size());
//    for(int i = 0; i < targetStart.size(); i++) {
//        EXPECT_EQ(targetStart.at(i), merged.at(i)->getStart());
//        EXPECT_EQ(targetLength.at(i), merged.at(i)->getLength());
//    }
//}
//
//TEST(reader, TimeProfilerTest) {
//    // single thread test
//    TimeProfiler & profiler = TimeProfiler::Instance();
//    profiler.Start("test");
//    sleep(2);
//    profiler.End("test");
//    profiler.Collect();
//    EXPECT_EQ(profiler.GetResultSize(), 1);
//    EXPECT_GE(profiler.Get("test"), 2000000000 - 50000000);
//    EXPECT_LE(profiler.Get("test"), 2000000000 + 50000000);
//    profiler.Reset();
//    EXPECT_EQ(profiler.GetResultSize(), 0);
//    // multi thread test
//    unsigned long nthreads = std::thread::hardware_concurrency() - 1;
//    std::vector<std::thread> worker;
//    for(int i = 0; i < nthreads; i++) {
//        worker.emplace_back([]{
//            TimeProfiler & profiler = TimeProfiler::Instance();
//            profiler.Start("test 0");
//            profiler.Start("test 1");
//            sleep(2);
//            profiler.Start("test 2");
//            profiler.End("test 1");
//            sleep(1);
//            profiler.End("test 2");
//            profiler.End("test 0");
//            profiler.Collect();
//        });
//    }
//    for(int i = 0; i < nthreads; i++) {
//        worker.at(i).join();
//    }
//
//    EXPECT_EQ(profiler.GetResultSize(), 3);
//    EXPECT_GE(profiler.Get("test 0"), 3000000000 * nthreads - 5000000);
//    EXPECT_LE(profiler.Get("test 0"), 3000000000 * nthreads + 5000000);
//    EXPECT_GE(profiler.Get("test 1"), 2000000000 * nthreads - 5000000);
//    EXPECT_LE(profiler.Get("test 1"), 2000000000 * nthreads + 5000000);
//    EXPECT_GE(profiler.Get("test 2"), 1000000000 * nthreads - 5000000);
//    EXPECT_LE(profiler.Get("test 2"), 1000000000 * nthreads + 5000000);
//    profiler.Reset();
//    EXPECT_EQ(profiler.GetResultSize(), 0);
//}
//
//
//TEST(reader, CountProfilerTest) {
//    // single thread test
//    CountProfiler & profiler = CountProfiler::Instance();
//    for(int i = 0; i < 5; i++) {
//        profiler.Count("test");
//    }
//    EXPECT_EQ(profiler.Get("test"), 5);
//    profiler.Reset();
//    // multi thread test
//    unsigned int nthreads = std::thread::hardware_concurrency();
//    std::vector<std::thread> worker;
//    for(int i = 0; i < nthreads; i++) {
//        worker.emplace_back([]{
//            CountProfiler & profiler = CountProfiler::Instance();
//            for(int i = 0; i < 10000; i++) {
//                profiler.Count("test");
//                profiler.Count("test 1");
//                profiler.Count("test 2");
//                profiler.Count("test 3");
//                profiler.Count("test 4");
//            }
//        });
//    }
//    for(int i = 0; i < nthreads; i++) {
//        worker.at(i).join();
//    }
//    EXPECT_EQ(profiler.Get("test"), 10000 * nthreads);
//    EXPECT_EQ(profiler.Get("test 1"), 10000 * nthreads);
//    EXPECT_EQ(profiler.Get("test 2"), 10000 * nthreads);
//    EXPECT_EQ(profiler.Get("test 3"), 10000 * nthreads);
//    EXPECT_EQ(profiler.Get("test 4"), 10000 * nthreads);
//    profiler.Print();
//    profiler.Reset();
//}
///* Returns a list of files in a directory (except the ones that begin with a dot) */
//
//void GetFilesInDirectory(std::vector<string> &out, const string &directory)
//{
//	DIR *dir;
//	class dirent *ent;
//	class stat st;
//
//	dir = opendir(directory.c_str());
//	while ((ent = readdir(dir)) != NULL) {
//		const string file_name = ent->d_name;
//		const string full_file_name = directory + "/" + file_name;
//
//		if (file_name[0] == '.')
//			continue;
//
//		if (stat(full_file_name.c_str(), &st) == -1)
//			continue;
//
//		const bool is_directory = (st.st_mode & S_IFDIR) != 0;
//
//		if (is_directory)
//			continue;
//
//		out.push_back(full_file_name);
//	}
//	closedir(dir);
//} // GetFilesInDirectory
//
//TEST(reader, LargeDatasetThroughputTest) {
//	string path = "/data/s1725-1/liyu/pixels_data/pixels-tpch-300/orders/v-0-order/";
//	std::vector<string> databases;
//	GetFilesInDirectory(databases, path);
//	std::vector<std::shared_ptr<VectorizedRowBatch>> results;
//	for(int i = 0; i < databases.size(); i++) {
//		auto dataset = databases[i];
//		auto footerCache = std::make_shared<PixelsFooterCache>();
//		auto builder = std::make_shared<PixelsReaderBuilder>();
//		auto storage = StorageFactory::getInstance()->getStorage(Storage::file);
//		auto pixelsReader = builder->setPath(dataset)->setStorage(storage)->setPixelsFooterCache(footerCache)->build();
//		PixelsReaderOption option;
//		option.setSkipCorruptRecords(true);
//		option.setTolerantSchemaEvolution(true);
//		option.setEnableEncodedColumnVector(true);
//
//		// includeCols comes from the caller of PixelsPageSource
//		std::vector<std::string> includeCols = pixelsReader->getFileSchema()->getFieldNames();
//		option.setIncludeCols(includeCols);
//		option.setRGRange(0, 1);
//		option.setQueryId(1);
//		auto pixelsRecordReader = pixelsReader->read(option);
//		while (true) {
//			std::shared_ptr<VectorizedRowBatch> v = pixelsRecordReader->readBatch(2048, false);
//			if (v->endOfFile) {
//				results.emplace_back(v);
//				break;
//			}
//		}
////		EXPECT_FALSE(v->endOfFile);
////		EXPECT_FALSE(pixelsRecordReader->isEndOfFile());
////		EXPECT_EQ(v->rowCount, 13);
////		for (const auto &col : v->cols) {
////			std::cout << "------" << std::endl;
////			col->print(v->rowCount);
////		}
////		std::shared_ptr<VectorizedRowBatch> v1 = pixelsRecordReader->readBatch(120, false);
////		EXPECT_TRUE(v1->endOfFile);
////		EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
////		EXPECT_EQ(v1->rowCount, 12);
////		std::cout << "------" << std::endl;
////		std::cout << "------" << std::endl;
////		std::cout << "------" << std::endl;
////		std::cout << "------" << std::endl;
////		for (const auto &col : v1->cols) {
////			std::cout << "------" << std::endl;
////			col->print(v1->rowCount);
////		}
////		std::shared_ptr<VectorizedRowBatch> v2 = pixelsRecordReader->readBatch(120, false);
////		EXPECT_TRUE(v2->endOfFile);
////		EXPECT_TRUE(pixelsRecordReader->isEndOfFile());
////		EXPECT_EQ(v2->rowCount, 0);
//	}
//	for(auto result: results) {
//		int a = rand() % 1 + 1;
//		for (const auto &col : result->cols) {
//			std::cout<<col->memoryUsage<<std::endl;
//		}
//	}
//}
//
//TEST(reader, LargeIOThroughputTest) {
//	string path = "/scratch/liyu/opt/pixels_file/pixels-tpch-300/orders/v-0-order/";
//	std::vector<string> databases;
//	GetFilesInDirectory(databases, path);
//	std::vector<std::shared_ptr<ByteBuffer>> results;
//	for(int i = 0; i < databases.size(); i++) {
//		auto dataset = databases[i];
//		auto localfs = std::make_shared<LocalFS>();
//		auto localReader = std::make_shared<PhysicalLocalReader>(localfs, dataset);
//		std::shared_ptr<ByteBuffer> bb = localReader->readFully(60*1024*1024);
//		results.emplace_back(bb);
//	}
//	for(auto result: results) {
//		int a = rand() % 1 + 1;
//		std::cout<<result->size()<<std::endl;
//	}
//}
TEST(reader, filterMaskTest) {
    PixelsBitMask filterMask(1025);
	filterMask.set(0, 1);
	filterMask.set(66, 1);
	filterMask.set(1023, 1);
	filterMask.set(1024, 1);
	filterMask.set();
	filterMask.set(0,0);
	int a = filterMask.get(0);
	int b = filterMask.get(66);
	int c = filterMask.get(2);
	int d = filterMask.get(1023);
	for(int i = 0; i < 1025; i++) {
		filterMask.set(i, 0);
	}
	bool isNone = filterMask.isNone();

}

static const uint32_t TestRowNum = 10;

template<class T> 
bool arrayEquals(T* a, T* b, int len) {
    for(int i = 0; i < len; ++i) {
        if(a[i] == b[i]) continue;
        std::cout << "Found a and b not equal at " << i << " " << a[i] << " " << b[i] << std::endl; 
        return false;
    }
    return true;
}

TEST(reader, runLengthTest) {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<long> dist;

    long* values = new long[TestRowNum];
    values[0] = 0;
    values[1] = -1;
    values[2] = -2;
    for (int i = 3; i < TestRowNum; i++)
    {
        values[i] = dist(e);
    }
    long* decoderValues = new long[TestRowNum];
    RunLenIntEncoder encoder(true, true);
    byte* bytes = new byte[TestRowNum * sizeof(long) * 2];
    int len = 0;
    encoder.encode(values, bytes, TestRowNum, len);
    std::shared_ptr<ByteBuffer> buffer = std::make_shared<ByteBuffer>(bytes, len, true);
    RunLenIntDecoder decoder(buffer, true);
    int i = 0;
    while (decoder.hasNext())
    {
        // std::cout << "decoding " << i << std::endl;
        decoderValues[i++] = decoder.next();
        // std::cout << "result: " << decoderValues[i - 1] << std::endl;
    }
    assert(arrayEquals<long>(decoderValues, values,TestRowNum));
    delete[] values;
    delete[] decoderValues;
}
