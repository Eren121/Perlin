#ifndef CHUNK_HPP
#define CHUNK_HPP

#include<vector>

class World;

class Chunk
{
public:
    Chunk(const World&);
    ~Chunk();

    int getW() const { return w; }
    int getH() const { return h; }

private:
    std::vector<float> height; // Stored as this:
    // 0,n          n,n
    //   7    8    9
    //   4    5    6
    //   1    2    3
    // 0,0          n,0

    int w, h;
};

#endif // CHUNK_HPP
