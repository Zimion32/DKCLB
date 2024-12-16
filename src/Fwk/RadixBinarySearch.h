#pragma once
template <class T>
class RadixBinarySearch
{
    private:
        U32 radix_count_;
        
    public:
        void Init()
        {
            radix_count_ = T::RadixCount();
            if(radix_count_ & 1) abort(); ///WARNING: COUNT MUST BE MULTIPLE OF 2.
        }
        U32 Search(Vx<T*>& input, U32 option, U64 key)
        {
            INT first = 0, last = input.size()-1;//SPECIAL: Control Parameters
            INT l = first, r = last;//Real traversal values.
            INT m = (l + r) / 2;
            if(l > r) return 0x80000000;///Empty: Not Found
            while(true){///2-Items or more...
                if(l == r){///if 1 item left to consider
                    if(key > input[l]->Value(option)){
                        return (r+1) | 0x80000000;
                    }else{
                        return (l+0) | ((key < input[l]->Value(option)) << 31);
                    }
                }
                if(key == input[m]->Value(option)){
                    return (m+0);
                }else{
                    if(key < input[m]->Value(option)){///examine to left of [m]
                        if(m == l) break;//ran out of things to search (2 items)
                        r = m-1;
                    }else{///examine to right of [m] (cannot run out here - must occur 2+ items)
                        l = m+1;
                    }
                    m = (l + r) / 2;
                }
            }
            return (m+0) | 0x80000000;
        }
        U32 Search(Vx<T* >& input, U32 option, U64 key)
        {
            INT first = 0, last = input.size()-1;//SPECIAL: Control Parameters
            INT l = first, r = last;//Real traversal values.
            INT m = (l + r) / 2;
            if(l > r) return 0x80000000;///Empty: Not Found
            while(true){///2-Items or more...
                if(l == r){///if 1 item left to consider
                    if(key > input[l]->Value(option)){
                        return (r+1) | 0x80000000;
                    }else{
                        return (l+0) | ((key < input[l]->Value(option)) << 31);
                    }
                }
                if(key == input[m]->Value(option)){
                    return (m+0);
                }else{
                    if(key < input[m]->Value(option)){///examine to left of [m]
                        if(m == l) break;//ran out of things to search (2 items)
                        r = m-1;
                    }else{///examine to right of [m] (cannot run out here - must occur 2+ items)
                        l = m+1;
                    }
                    m = (l + r) / 2;
                }
            }
            return (m+0) | 0x80000000;
        }
};
template <class T>
class RadixBinaryRangeSearch
{
    private:
        RadixBinarySearch<T> searcher_;
        
    public:
        void Init()
        {
            searcher_.Init();
        }
        SPoint Search(Vx<T*>& input, U32 option, U64 key_beg, U64 key_end)
        {
            U32 first = searcher_.Search(input, option, key_beg);
            U32 last = searcher_.Search(input, option, key_end);
            if(~first & 0x80000000){//if found
                while((first & 0x7FFFFFFF) 
                && (input[(first & 0x7FFFFFFF) - 0]->Value(option) 
                 == input[(first & 0x7FFFFFFF) - 1]->Value(option))){
                    first--;
                }
            }
            if(~last & 0x80000000){//if found
                while((((last & 0x7FFFFFFF) + 1) < input.size())
                && (input[(last & 0x7FFFFFFF) + 0]->Value(option) 
                 == input[(last & 0x7FFFFFFF) + 1]->Value(option))){
                    last++;
                }
                last++; //if vector ends in element that's found, we extend the range by +1
            }
            first &= 0x7FFFFFFF; //ranges don't include not present bit
            last &= 0x7FFFFFFF; //ranges don't include not present bit
            return SPoint(first, last);
        }
        SPoint Search(Vx<T* >& input, U32 option, U64 key_beg, U64 key_end)
        {
            U32 first = searcher_.Search(input, option, key_beg);
            U32 last = searcher_.Search(input, option, key_end);
            if(~first & 0x80000000){//if found
                while((first & 0x7FFFFFFF) 
                && (input[(first & 0x7FFFFFFF) - 0]->Value(option) 
                 == input[(first & 0x7FFFFFFF) - 1]->Value(option))){
                    first--;
                }
            }
            if(~last & 0x80000000){//if found
                while((((last & 0x7FFFFFFF) + 1) < input.size())
                && (input[(last & 0x7FFFFFFF) + 0]->Value(option) 
                 == input[(last & 0x7FFFFFFF) + 1]->Value(option))){
                    last++;
                }
                last++; //if vector ends in element that's found, we extend the range by +1
            }
            first &= 0x7FFFFFFF; //ranges don't include not present bit
            last &= 0x7FFFFFFF; //ranges don't include not present bit
            return SPoint(first, last);
        }
};
