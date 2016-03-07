#include "helper.h"

Helper::Helper()
{

}

int Helper::getMaxValue(QList<int> values)
{
    return *std::max_element(values.begin(), values.end());
}

