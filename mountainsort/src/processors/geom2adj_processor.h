/******************************************************
** See the accompanying README and LICENSE files
** Author(s): Jeremy Magland
** Created: 5/10/2016
*******************************************************/

#ifndef GEOM2ADJ_H
#define GEOM2ADJ_H

#include "msprocessor.h"

class geom2adj_ProcessorPrivate;
class geom2adj_Processor : public MSProcessor
{
public:
    friend class geom2adj_ProcessorPrivate;
    geom2adj_Processor();
    virtual ~geom2adj_Processor();

    bool check(const QMap<QString,QVariant> &params);
    bool run(const QMap<QString,QVariant> &params);
private:
    geom2adj_ProcessorPrivate *d;
};

#endif // GEOM2ADJ_H

