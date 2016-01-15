cmake_minimum_required(VERSION 2.6)

project(Vetorial)
add_definitions("-g")
add_executable(Indexer Postlist.cpp Vocabulary.cpp Indexer.cpp MainIndexer.cpp)
add_executable(Reader Postlist.cpp Vocabulary.cpp VectorModel.cpp Query.cpp MainReader.cpp) 