#include "curves.h"
#include "theDataset.h"


Curves::Curves(int lines, int points)
{
	m_lineCount = lines;
	m_pointCount = points;
	m_linePointers = new int[lines+1];
	m_linePointers[lines] = points;
	m_reverse = new int[points];
	m_inBox.resize(lines, sizeof(bool));
	for (int i = 0; i < lines ; ++i)
	{
		m_inBox[i] = 0;
	}
}

Curves::~Curves()
{
	delete[] m_linePointers;
	delete[] m_pointArray;
	delete[] m_lineArray;
	delete[] m_reverse;
}

int Curves::getPointsPerLine(int line) 
{
	return (m_linePointers[line+1] - m_linePointers[line]) ;
}

int Curves::getStartIndexForLine(int line) 
{
	return m_linePointers[line];
}


void Curves::calculateLinePointers()
{
	TheDataset::printTime();
	printf("calculate line pointers\n");
	int pc = 0;
	int lc = 0;
	int tc = 0;
	for (int i = 0 ; i < m_lineCount ; ++i)
	{
		m_linePointers[i] = tc;
		lc = m_lineArray[pc];
		tc += lc;
		pc += (lc + 1);
	}
	
	lc = 0;
	pc = 0;
	
	
	for ( int i = 0 ; i < m_pointCount ; ++i)
	{
		if ( i == m_linePointers[lc+1]) ++lc;
		m_reverse[i] = lc;
	}
}

int Curves::getLineForPoint(int point)
{
	return m_reverse[point];
}

void Curves::toggleEndianess()
{
	TheDataset::printTime();
	printf("toggle Endianess\n");
	
	wxUint8 *pointbytes = (wxUint8*)m_pointArray;
	wxUint8 temp;
	for ( int i = 0 ; i < m_lengthPoints*4; i +=4)
	{
		temp  = pointbytes[i];
		pointbytes[i] = pointbytes[i+3]; 
		pointbytes[i+3] = temp;
		temp  = pointbytes[i+1];
		pointbytes[i+1] = pointbytes[i+2]; 
		pointbytes[i+2] = temp;
	}

	wxUint8 *linebytes = (wxUint8*)m_lineArray;
	for ( int i = 0 ; i < m_lengthLines*4; i +=4)
	{
		temp  = linebytes[i];
		linebytes[i] = linebytes[i+3]; 
		linebytes[i+3] = temp;
		temp  = linebytes[i+1];
		linebytes[i+1] = linebytes[i+2]; 
		linebytes[i+2] = temp;
	}
}

void Curves::createColorArray()
{
	TheDataset::printTime();
	printf("create color arrays\n");

	int pc = 0;
    float r,g,b, rr, gg, bb;
    float x1,x2,y1,y2,z1,z2;
    float lastx, lasty, lastz;
    for ( int i = 0 ; i < getLineCount() ; ++i )
    {
    	//pc = getStartIndexForLine(i)*3;
        x1 = m_pointArray[pc];
        y1 = m_pointArray[pc+1];
        z1 = m_pointArray[pc+2];
        x2 = m_pointArray[pc + getPointsPerLine(i)*3 - 3];
        y2 = m_pointArray[pc + getPointsPerLine(i)*3 - 2];
        z2 = m_pointArray[pc + getPointsPerLine(i)*3 - 1];
        
        r = (x1) - (x2);
        g = (y1) - (y2);
        b = (z1) - (z2);
        if (r < 0.0) r *= -1.0 ;
        if (g < 0.0) g *= -1.0 ;
        if (b < 0.0) b *= -1.0 ;
        
        float norm = sqrt(r*r + g*g + b*b);
        r *= 1.0/norm;
        g *= 1.0/norm;
        b *= 1.0/norm;

        lastx = lasty = lastz = 0.0;
        
        for (int j = 0; j < getPointsPerLine(i) ; ++j )
        {
        	rr = lastx - m_pointArray[pc];
            gg = lasty - m_pointArray[pc+1];
            bb = lastz - m_pointArray[pc+2];
            lastx = m_pointArray[pc];
            lasty = m_pointArray[pc+1];
            lastz = m_pointArray[pc+2];
            if (rr < 0.0) rr *= -1.0 ;
            if (gg < 0.0) gg *= -1.0 ;
            if (bb < 0.0) bb *= -1.0 ;
            float norm = sqrt(rr*rr + gg*gg + bb*bb);
            rr *= 1.0/norm;
            gg *= 1.0/norm;
            bb *= 1.0/norm;

        	m_normalArray[pc] = rr;
        	m_normalArray[pc+1] = gg;
        	m_normalArray[pc+2] = bb;
        	
        	m_colorArray[pc] = r;
	        m_colorArray[pc+1] = g;
	        m_colorArray[pc+2] = b;
	        pc += 3;
        }
    }

}

void Curves::buildkDTree()
{
	m_kdTree = new KdTree(m_pointCount, m_pointArray);
}

void Curves::resetLinesShown()
{
	for (int i = 0; i < m_lineCount ; ++i)
	{
		m_inBox[i] = 0;
	}
}

void Curves::updateLinesShown(std::vector<std::vector<SelectionBox*> > boxes)
{
	for (uint i = 0 ; i != boxes.size() ; ++i)
	{
		bool dirty = false;
		for (uint j = 0 ; j < boxes[i].size() ; ++j)
		{
			if (boxes[i][j]->isDirty()) dirty = true; 
		}
		if (dirty)
		{
			boxes[i][0]->m_inBox = getLinesShown(boxes[i][0]);
			boxes[i][0]->notDirty();

			for (uint j = 1 ; j < boxes[i].size() ; ++j)
			{
				if  (boxes[i][j]->isDirty()) {
					boxes[i][j]->m_inBox = getLinesShown(boxes[i][j]);
					boxes[i][j]->notDirty();
				}
				for (int k = 0 ; k <m_lineCount ; ++k)
					boxes[i][0]->m_inBox[k] = boxes[i][0]->m_inBox[k] & boxes[i][j]->m_inBox[k];  
			}

		}
	}
	resetLinesShown();
	for (uint i = 0 ; i < boxes.size() ; ++i)
	{
		for (int k = 0 ; k <m_lineCount ; ++k)
			m_inBox[k] = m_inBox[k] | boxes[i][0]->m_inBox[k];
	}
}

std::vector<bool> Curves::getLinesShown(SelectionBox* box)
{
	Vector3fT vpos = box->getCenter();
	Vector3fT vsize = box->getSize();
	resetLinesShown();
	m_boxMin = new float[3];
	m_boxMax = new float[3];
	m_boxMin[0] = vpos.s.X - vsize.s.X/2;
	m_boxMax[0] = vpos.s.X + vsize.s.X/2;
	m_boxMin[1] = vpos.s.Y - vsize.s.Y/2;
	m_boxMax[1] = vpos.s.Y + vsize.s.Y/2;
	m_boxMin[2] = vpos.s.Z - vsize.s.Z/2;
	m_boxMax[2] = vpos.s.Z + vsize.s.Z/2;

	boxTest(0, m_pointCount-1, 0);
	return m_inBox;
}

void Curves::boxTest(int left, int right, int axis)
{
	if (left > right) return;
	int root = left + ((right-left)/2);
	int axis1 = (axis+1) % 3;
	int pointIndex = m_kdTree->m_tree[root]*3;
	
	if (m_pointArray[pointIndex + axis] < m_boxMin[axis]) {
		boxTest(root +1, right, axis1);
	}
	else if (m_pointArray[pointIndex + axis] > m_boxMax[axis]) {
		boxTest(left, root-1, axis1);
	}
	else {
		int axis2 = (axis+2) % 3;
		if (	m_pointArray[pointIndex + axis1] <= m_boxMax[axis1] &&
				m_pointArray[pointIndex + axis1] >= m_boxMin[axis1] &&
				m_pointArray[pointIndex + axis2] <= m_boxMax[axis2] &&
				m_pointArray[pointIndex + axis2] >= m_boxMin[axis2] )
		{
			m_inBox[getLineForPoint(m_kdTree->m_tree[root])] = 1;
		}
		boxTest(left, root -1, axis1);
		boxTest(root+1, right, axis1);
	}
}