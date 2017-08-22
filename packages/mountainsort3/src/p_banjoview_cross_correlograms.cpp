#include "p_banjoview_cross_correlograms.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <diskreadmda.h>
#include <mlvector.h>
#include "get_sort_indices.h"

bool p_banjoview_cross_correlograms(QString firings_path, QString histograms_out, P_banjoview_cross_correlograms_opts opts)
{

    DiskReadMda firings(firings_path);

    //get the sorted times and labels
    MLVector<double> times(firings.N2());
    MLVector<int> labels(firings.N2());
    {
        MLVector<double> times_unsorted(firings.N2());
        MLVector<int> labels_unsorted(firings.N2());
        for (bigint i = 0; i < firings.N2(); i++) {
            times_unsorted[i] = firings.value(1, i);
            labels_unsorted[i] = firings.value(2, i);
        }
        MLVector<bigint> sort_inds = get_sort_indices(times_unsorted);
        for (bigint i = 0; i < firings.N2(); i++) {
            times[i] = times_unsorted[sort_inds[i]];
            labels[i] = labels_unsorted[sort_inds[i]];
        }
    }

    int K = MLCompute::max(labels);
    if (opts.clusters.isEmpty()) {
        for (int k = 1; k <= K; k++)
            opts.clusters << k;
    }

    QVector<int> k1s, k2s;
    if (opts.mode == Autocorrelograms) {
        for (int ii = 0; ii < opts.clusters.count(); ii++) {
            k1s << opts.clusters[ii];
            k2s << opts.clusters[ii];
        }
    }
    else if (opts.mode == Matrix_of_cross_correlograms) {
        for (int i1 = 0; i1 < opts.clusters.count(); i1++) {
            for (int i2 = 0; i2 < opts.clusters.count(); i2++) {
                k1s << opts.clusters[i1];
                k2s << opts.clusters[i2];
            }
        }
    }

    //set up the histogram bins
    int R = ceil((opts.max_dt_msec) / (opts.bin_size_msec));
    bigint num_bins = 2 * R;
    double tmin = -R * opts.bin_size_msec / 1000 * opts.samplerate;
    double tmax = R * opts.bin_size_msec / 1000 * opts.samplerate;
    double bin_size = opts.bin_size_msec / 1000 * opts.samplerate;

    //initialize the histograms with zeros
    QMap<bigint, QVector<double> > histogram_counts;
    QVector<double> zeros(num_bins, 0);
    for (int ii = 0; ii < k1s.count(); ii++) {
        int k1 = k1s[ii];
        int k2 = k2s[ii];
        int num = (k1 - 1) + K * (k2 - 1);
        histogram_counts[num] = zeros;
    }

    qDebug().noquote() << "k1s:" << k1s;
    qDebug().noquote() << "k2s:" << k1s;
    qDebug() << times.count();

    bigint jjj_last = 0;
    for (bigint ii = 0; ii < times.count(); ii++) {
        double t1 = times[ii];
        int k1 = labels[ii];
        bigint jjj = jjj_last;
        while ((jjj + 1 < times.count()) && (times[jjj] < t1 + tmin))
            jjj++;
        jjj_last = jjj;
        while ((jjj < times.count()) && (t1 + tmin <= times[jjj]) && (times[jjj] < t1 + tmax)) {
            double t2 = times[jjj];
            int k2 = labels[jjj];
            bigint num = (k1 - 1) + K * (k2 - 1);
            if (histogram_counts.contains(num)) {
                double dt = t2 - t1;
                int bin_index = (dt - tmin) / bin_size;
                histogram_counts[num][bin_index]++;
            }
            jjj++;
        }
    }

    QJsonArray histograms;
    for (int ii = 0; ii < k1s.count(); ii++) {
        QJsonObject histogram;
        histogram["k1"] = k1s[ii];
        histogram["k2"] = k2s[ii];
        histogram["tmin_msec"] = tmin * 1000 / opts.samplerate;
        histogram["bin_size_msec"] = bin_size * 1000 / opts.samplerate;
        int num = (k1s[ii] - 1) + K * (k2s[ii] - 1);
        QJsonArray counts0;
        for (int a = 0; a < num_bins; a++) {
            counts0 << histogram_counts[num][a];
        }
        histogram["counts"] = counts0;
        histograms.push_back(histogram);
    }

    QJsonObject output;
    output["histograms"] = histograms;
    QString json_output = QJsonDocument(output).toJson(QJsonDocument::Compact);
    return TextFile::write(histograms_out, json_output);
}
