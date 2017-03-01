/******************************************************
** See the accompanying README and LICENSE files
** Author(s): Jeremy Magland
** Created: 8/24/2016
*******************************************************/
#ifndef SSLVTimeSpanPLUGIN_H
#define SSLVTimeSpanPLUGIN_H

#include "mvmainwindow.h"

#include <QThread>

class SSLVTimeSpanPluginPrivate;
class SSLVTimeSpanPlugin : public MVAbstractPlugin {
public:
    friend class SSLVTimeSpanPluginPrivate;
    SSLVTimeSpanPlugin();
    virtual ~SSLVTimeSpanPlugin();

    QString name() Q_DECL_OVERRIDE;
    QString description() Q_DECL_OVERRIDE;
    void initialize(MVMainWindow* mw) Q_DECL_OVERRIDE;

private:
    SSLVTimeSpanPluginPrivate* d;
};

class SSLVTimeSpanFactory : public MVAbstractViewFactory {
    Q_OBJECT
public:
    SSLVTimeSpanFactory(MVMainWindow* mw, QObject* parent = 0);
    QString id() const Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
    QString title() const Q_DECL_OVERRIDE;
    MVAbstractView* createView(MVAbstractContext* context) Q_DECL_OVERRIDE;
private slots:
    //void openClipsForTemplate();
};

#endif // SSLVTimeSpanPLUGIN_H
