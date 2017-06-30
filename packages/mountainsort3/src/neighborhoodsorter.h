#ifndef NEIGHBORHOODSORTER_H
#define NEIGHBORHOODSORTER_H

#include "mda32.h"
#include "p_mountainsort3.h"

class NeighborhoodSorterPrivate;
class NeighborhoodSorter {
public:
    friend class NeighborhoodSorterPrivate;
    NeighborhoodSorter();
    virtual ~NeighborhoodSorter();

    void setNumThreads(int num_threads);
    void setOptions(P_mountainsort3_opts opts);
    void addTimeChunk(bigint t, const Mda32& X, bigint padding_left, bigint padding_right);
    void sort();
    QVector<double> times() const;
    QVector<int> labels() const;
    Mda32 templates() const;

private:
    NeighborhoodSorterPrivate* d;
};

class DiskBackedMda32 {
public:
    DiskBackedMda32();
    DiskBackedMda32(const Mda32 &X);
    virtual ~DiskBackedMda32();
    void store(const Mda32 &X);
    void retrieve(Mda32 &X) const;
    void remove();
private:
    QString m_tmp_path;
};

#endif // NEIGHBORHOODSORTER_H