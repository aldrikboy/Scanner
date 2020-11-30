#include "SampleCollector.hpp"

class StorageManager
{
public:
    static void Setup();
    static void ReadSampleCount();
    static void WriteSampleCount();

    static SampleCollection PopSample();
    static bool PushSample();
};