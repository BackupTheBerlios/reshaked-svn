//
// C++ Interface: route_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDROUTE_EDITOR_H
#define RESHAKEDROUTE_EDITOR_H

#include <Qt/qwidget.h>
#include <vector>
#include <Qt/qpixmap.h>


namespace ReShaked {

/**
	@author red <red@killy>
*/
class RouteEditor : public QWidget {
	
	Q_OBJECT
public:
	
	struct Skin {
		
		QPixmap bg;
		QSize node_size;
		QColor line_bg;
		QColor line_connected;
		QPixmap cursor_over;
	};
	
private:	
	
	Skin skin;
	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void leaveEvent ( QEvent * );
	
	int nodes;
	
	std::vector<bool> connections;
	int cursor_over;
	
	int get_ofs(int p_node,int p_to_node);
	int get_click_pos(QPoint p_point);
	
signals:	
	
	void matrix_changed_signal();
public:	
	
	
	void clear_connections();
	
	void connect_nodes(int p_node,int p_to_node);
	void disconnect_nodes(int p_node,int p_from_node);
	bool are_connected(int p_node,int p_with_node);	
	
	RouteEditor(QWidget *p_parent,int p_nodes,const Skin& p_skin);
	
	~RouteEditor();

};

}

#endif
