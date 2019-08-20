#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/fusion/support/pair.hpp>
// debugging
#include <iostream>


struct non_persistent_t {};
struct persistent_t {};
static const non_persistent_t    non_persistent    = non_persistent_t();
//!Value to indicate that the unordered_map must
//!be non-persistent
static const persistent_t    persistent    = persistent_t();
//!Value to indicate that the unordered_map must
//!be persistent 

template<typename KeyType, typename MappedType>
class unordered_map{
   const char *ManagedFile;
   const char *map_name;

   typedef std::pair<const KeyType, MappedType> ValueType;
   typedef boost::interprocess::allocator<ValueType, boost::interprocess::managed_mapped_file::segment_manager> ShmemAllocator;
   typedef boost::interprocess::managed_mapped_file managed_segment;
   typedef boost::unordered_map
      < KeyType               , MappedType
      , boost::hash<KeyType>  ,std::equal_to<KeyType>
      , ShmemAllocator>
   MyHashMap;
   MyHashMap *myhashmap;
   boost::interprocess::managed_mapped_file::handle_t file_handler;
   long file_size;
   bool is_persistent = true;
   boost::interprocess::managed_mapped_file segment;
   public:

    unordered_map(persistent_t,const char *FileName, const char *map_nam ,long size, int num_buckets);
    unordered_map(non_persistent_t,const char *FileName, int size, int num_buckets);
    bool insert(KeyType,MappedType);
    MappedType get(KeyType);
};
template<typename KeyType, typename MappedType>
unordered_map<KeyType,MappedType>::unordered_map(persistent_t p,const char *FileName, const char *map_nam, long size,int num_buckets){
    ManagedFile = FileName;
    file_size = size;
    is_persistent = true;
    map_name = map_nam;
    segment = boost::interprocess::managed_mapped_file(boost::interprocess::open_or_create, ManagedFile, file_size);
    
    myhashmap = segment.find_or_construct<MyHashMap>(map_name)
      ( num_buckets, boost::hash<int>(), std::equal_to<int>()    
      , segment.get_allocator<ValueType>());     
}


template<typename KeyType, typename MappedType>
unordered_map<KeyType,MappedType>::unordered_map(non_persistent_t p,const char *FileName, int size,int num_buckets){
    ManagedFile = FileName;
    file_size = size;
    is_persistent = false;
    boost::interprocess::managed_shared_memory segment(boost::interprocess::create_only, ManagedFile, file_size);
   myhashmap = segment.construct<MyHashMap>("MyHashMap")  
      ( num_buckets, boost::hash<int>(), std::equal_to<int>()                  
      , segment.get_allocator<ValueType>());                       
}


template<typename KeyType, typename MappedType>
bool unordered_map<KeyType,MappedType>::insert(KeyType key,MappedType data){
    try{
        myhashmap->insert(ValueType(key,data));
      //segment.flush();
    }catch(const boost::interprocess::bad_alloc &){
        return false;
    }
    return true;
}
template<typename KeyType, typename MappedType>
MappedType unordered_map<KeyType,MappedType>::get(KeyType key){
    auto mydata = myhashmap->find(key);
    if ( mydata == myhashmap->end() ){
        return nullptr;
    }
    else{
        return mydata->second;
    }
}