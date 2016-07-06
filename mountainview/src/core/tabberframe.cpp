/******************************************************
** See the accompanying README and LICENSE files
** Author(s): Jeremy Magland
** Created: 6/20/2016
*******************************************************/

#include "tabberframe.h"

#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>
#include <QToolButton>

class TabberFramePrivate {
public:
    TabberFrame* q;
    MVAbstractView* m_view;
    QString m_container_name;
    QToolBar* m_toolbar;
    QAction* m_recalc_action;
    QAction* m_never_recalc_action;
    QAction* m_move_up_action;
    QAction* m_move_down_action;
    QAction* m_pop_out_action;
    QAction* m_pop_in_action;

    void update_action_visibility();
    static QList<QAction*> find_actions_of_type(QList<QAction*> actions, QString str);
};

TabberFrame::TabberFrame(MVAbstractView* view)
{
    d = new TabberFramePrivate;
    d->q = this;
    d->m_view = view;
    d->m_toolbar = new QToolBar;
    {
        QAction* A = new QAction(QIcon(":/images/calculator.png"), "", this);
        A->setToolTip("Recalculate this view");
        QObject::connect(A, SIGNAL(triggered(bool)), view, SLOT(recalculate()));
        d->m_recalc_action = A;
    }
    {
        QAction* A = new QAction(QIcon(":/images/calculator-no.png"), "", this);
        A->setToolTip("Lock this view so that it will not auto-recalculate");
        QObject::connect(A, SIGNAL(triggered(bool)), view, SLOT(neverSuggestRecalculate()));
        d->m_never_recalc_action = A;
    }
    {
        QAction* A = new QAction(QIcon(":/images/move-up.png"), "", this);
        A->setToolTip("Move view to other container");
        QObject::connect(A, SIGNAL(triggered(bool)), this, SIGNAL(signalMoveToOtherContainer()));
        d->m_move_up_action = A;
    }
    {
        QAction* A = new QAction(QIcon(":/images/move-down.png"), "", this);
        A->setToolTip("Move view to other container");
        QObject::connect(A, SIGNAL(triggered(bool)), this, SIGNAL(signalMoveToOtherContainer()));
        d->m_move_down_action = A;
    }
    {
        QAction* A = new QAction(QIcon(":/images/pop-out.png"), "", this);
        A->setToolTip("Float this view");
        QObject::connect(A, SIGNAL(triggered(bool)), this, SIGNAL(signalPopOut()));
        d->m_pop_out_action = A;
    }
    {
        QAction* A = new QAction(QIcon(":/images/pop-in.png"), "", this);
        A->setToolTip("Dock this view");
        QObject::connect(A, SIGNAL(triggered(bool)), this, SIGNAL(signalPopIn()));
        d->m_pop_in_action = A;
    }

    QToolButton* tool_button = new QToolButton;
    QMenu* menu = new QMenu;
    menu->addActions(d->find_actions_of_type(view->actions(), ""));
    tool_button->setMenu(menu);
    tool_button->setIcon(QIcon(":/images/gear.png"));
    tool_button->setPopupMode(QToolButton::InstantPopup);
    QWidget* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QList<QAction*> toolbar_actions = d->find_actions_of_type(view->actions(), "toolbar");
    foreach (QAction* a, toolbar_actions) {
        d->m_toolbar->addAction(a);
    }

    d->m_toolbar->addWidget(spacer);

    d->m_toolbar->addAction(d->m_recalc_action);
    d->m_toolbar->addAction(d->m_never_recalc_action);
    d->m_toolbar->addAction(d->m_move_up_action);
    d->m_toolbar->addAction(d->m_move_down_action);
    d->m_toolbar->addAction(d->m_pop_out_action);
    d->m_toolbar->addAction(d->m_pop_in_action);

    d->m_toolbar->addWidget(tool_button);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    vlayout->addWidget(d->m_toolbar);
    vlayout->addWidget(view);
    this->setLayout(vlayout);

    QObject::connect(view, SIGNAL(recalculateSuggestedChanged()), this, SLOT(slot_update_action_visibility()));
    d->update_action_visibility();

    QObject::connect(view, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
}

TabberFrame::~TabberFrame()
{
    delete d;
}

MVAbstractView* TabberFrame::view()
{
    return d->m_view;
}

void TabberFrame::setContainerName(QString name)
{
    if (d->m_container_name == name)
        return;
    d->m_container_name = name;
    d->update_action_visibility();
}

void TabberFrame::slot_update_action_visibility()
{
    d->update_action_visibility();
}

void TabberFramePrivate::update_action_visibility()
{
    if (m_view->recalculateSuggested()) {
        m_recalc_action->setVisible(true);
        m_never_recalc_action->setVisible(true);
    }
    else {
        m_recalc_action->setVisible(false);
        m_never_recalc_action->setVisible(false);
    }

    if (m_container_name == "north") {
        m_move_up_action->setVisible(false);
        m_move_down_action->setVisible(true);
        m_pop_out_action->setVisible(true);
        m_pop_in_action->setVisible(false);
    }
    else if (m_container_name == "south") {
        m_move_up_action->setVisible(true);
        m_move_down_action->setVisible(false);
        m_pop_out_action->setVisible(true);
        m_pop_in_action->setVisible(false);
    }
    else {
        m_move_up_action->setVisible(false);
        m_move_down_action->setVisible(false);
        m_pop_out_action->setVisible(false);
        m_pop_in_action->setVisible(true);
    }
}

QList<QAction*> TabberFramePrivate::find_actions_of_type(QList<QAction*> actions, QString str)
{
    QList<QAction*> ret;
    for (int i = 0; i < actions.count(); i++) {
        if (actions[i]->property("action_type").toString() == str) {
            ret << actions[i];
        }
    }
    return ret;
}