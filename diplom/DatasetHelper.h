/*
 * DatasetHelper.h
 *
 *  Created on: 27.07.2008
 *      Author: ralph
 */
#ifndef DATASETHELPER_H_
#define DATASETHELPER_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/textfile.h"
#include "wx/file.h"
#include "wx/image.h"
#include "wx/wfstream.h"
#include "wx/datstrm.h"
#include "wx/txtstrm.h"
#include "wx/xml/xml.h"

#include <vector>
#include "ArcBall.h"

#include "datasetInfo.h"
#include "selectionBox.h"

#include "mainFrame.h"
#include "theScene.h"
#include "splinePoint.h"

#include "AnatomyHelper.h"
#include "ShaderHelper.h"

class MainFrame;
class DatasetInfo;
class TheScene;
class AnatomyHelper;
class ShaderHelper;
class SplinePoint;
class SelectionBox;

class DatasetHelper {
public:
	DatasetHelper(MainFrame*);
	virtual ~DatasetHelper();

	bool load(int index = 0, wxString filename = wxT(""),
			float threshold = 0.0, bool active = true, bool showFS = true, bool useTex = true);
	void finishLoading(DatasetInfo*);
	bool loadScene(wxString filename);
	bool loadTextFile(wxString*, wxString);
	bool fileNameExists(wxString filename);

	//! Saves the current scene to an xml file
	void save(wxString filename);

	std::vector<std::vector<SelectionBox*> > getSelectionBoxes();
	void updateAllSelectionBoxes();
	Vector3fT mapMouse2World(int, int);
	Vector3fT mapMouse2WorldBack(int, int);
	void updateTreeDims();
	void updateTreeDS(int);

	bool invertFibers() {fibersInverted = !fibersInverted; return fibersInverted;};

	void createIsoSurface();
	/*
	 * Called from MainFrame when a kdTree thread signals it's finished
	 */
	void treeFinished();

	/*
	 * Helper functions
	 */
	void printTime();
	void printwxT(wxString);
	/*
	 * Check for GL error
	 */
	bool GLError();
	void printGLError(wxString function = wxT(""));

	void updateView(float x, float y, float z);

	void changeZoom(int z);
	void moveScene(int x, int y);

	void doMatrixManipulation();

	/////////////////////////////////////////////////////////////////////////////////
	// general info about the datasets
	/////////////////////////////////////////////////////////////////////////////////
	int rows;
	int columns;
	int frames;
	unsigned int countFibers;
	wxString lastError;
	// last path from loading, load file dialog will open with it
	wxString lastPath;

	bool anatomy_loaded;
	bool fibers_loaded;
	bool vectors_loaded;
	bool surface_loaded;
	bool surface_isDirty;

	/////////////////////////////////////////////////////////////////////////////////
	// state variables for rendering
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4fT m_transform;
	bool useVBO;
	GLenum lastGLError;
	int quadrant;
	//! if set the shaders will be reloaded during next render() call
	bool scheduledReloadShaders;
	/////////////////////////////////////////////////////////////////////////////////
	// state variables for menu entries
	/////////////////////////////////////////////////////////////////////////////////
	bool showSagittal;
	bool showCoronal;
	bool showAxial;

	float xSlize;
	float ySlize;
	float zSlize;
	// lighting for fibers
	bool lighting;
	// ignore threshold for textures on meshes
	bool blendTexOnMesh;
	// show the lic texture on spline surface
	bool use_lic;
	// draw vectors as small lines on spline surface
	bool drawVectors;
	// normal direction of the spline surface
	float normalDirection;
	bool fibersInverted;
	bool useFakeTubes;
	bool filterIsoSurf;
	int colorMap;
	/////////////////////////////////////////////////////////////////////////////////
	// pointers to often used objects
	/////////////////////////////////////////////////////////////////////////////////
	SplinePoint* lastSelectedPoint;
	SelectionBox* lastSelectedBox;
	SelectionBox* boxAtCrosshair;
	AnatomyHelper* anatomyHelper;
	ShaderHelper* shaderHelper;
	MainFrame* mainFrame;
	TheScene* scene;

	bool boxLockIsOn;
	bool semaphore;
	int threadsActive;
	/////////////////////////////////////////////////////////////////////////////////
	// pointers to often used objects
	/////////////////////////////////////////////////////////////////////////////////
	bool m_isDragging;
	bool m_isrDragging;
	bool m_ismDragging;
	float zoom;
	int xMove;
	int yMove;



private:

};

#define ID_KDTREE_FINISHED	50

#endif /* DATASETHELPER_H_ */
