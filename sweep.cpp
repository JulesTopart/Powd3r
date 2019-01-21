#include "sweep.h"


float getMin(std::vector<Line2D> lines) {
    float x_min;
    float x_max;
    if (lines.size() != 0) {
        x_min = lines[0].a.x();
        x_max = lines[0].a.x();

        for (int k(0); k < lines.size(); k++) {
            Line2D line = lines[k];
            if (line.a.x() < x_min) x_min = line.a.x();
            if (line.a.x() > x_max) x_max = line.a.x();
            if (line.b.x() < x_min) x_min = line.b.x();
            if (line.b.x() > x_max) x_max = line.b.x();
        }
    }
    return x_min;
}

float getMinY(std::vector<Line2D> lines) {
    float x_min;
    float x_max;
    if (lines.size() != 0) {
        x_min = lines[0].a.y();
        x_max = lines[0].a.y();

        for (int k(0); k < lines.size(); k++) {
            Line2D line = lines[k];
            if (line.a.y() < x_min) x_min = line.a.y();
            if (line.a.y() > x_max) x_max = line.a.y();
            if (line.b.y() < x_min) x_min = line.b.y();
            if (line.b.y() > x_max) x_max = line.b.y();
        }
    }
    return x_min;
}

float getMax(std::vector<Line2D> lines) {
    float x_min;
    float x_max;
    if (lines.size() != 0) {
        x_min = lines[0].a.x();
        x_max = lines[0].a.x();

        for (int k(0); k < lines.size(); k++) {
            Line2D line = lines[k];
            if (line.a.x() < x_min) x_min = line.a.x();
            if (line.a.x() > x_max) x_max = line.a.x();
            if (line.b.x() < x_min) x_min = line.b.x();
            if (line.b.x() > x_max) x_max = line.b.x();
        }
    }
    return x_min;
}

float getMaxY(std::vector<Line2D> lines) {
    float x_min;
    float x_max;
    if (lines.size() != 0) {
        x_min = lines[0].a.y();
        x_max = lines[0].a.y();

        for (int k(0); k < lines.size(); k++) {
            Line2D line = lines[k];
            if (line.a.y() < x_min) x_min = line.a.y();
            if (line.a.y() > x_max) x_max = line.a.y();
            if (line.b.y() < x_min) x_min = line.b.y();
            if (line.b.y() > x_max) x_max = line.b.y();
        }
    }
    return x_max;
}


 /************************/
 /*	     NozzeLine		 */
 /*						 */
 /************************/

//Operator
bool operator<(NozzleLine const &a, NozzleLine const& b)
{
    return a.smallerThan(b);
}

bool operator>(NozzleLine const &a, NozzleLine const& b)
{
    return a.biggerThan(b);
}


bool NozzleLine::smallerThan(NozzleLine const& b) const
{
    if (y < b.y)   // Si les heures sont différentes
        return true;
    else              //Si tout est égal, alors l'objet n'est pas plus petit que b
        return false;
}

bool NozzleLine::biggerThan(NozzleLine const& b) const
{
    if (y > b.y)   // Si les heures sont différentes
        return true;
    else              //Si tout est égal, alors l'objet n'est pas plus petit que b
        return false;
}

void NozzleLine::drawLine() {
    float spacing(25.4f / 96.0f);
    //calcMinMax();
    if (line_coll.size() != 0) {
        for (float xpos = 0.0; xpos < 50.0; xpos += spacing) {
            bool active = false;
            for (int j(0); j < line_coll.size(); j++) {
                if (xpos >= line_coll[j].a.x() && xpos <= line_coll[j].b.x() || xpos >= line_coll[j].b.x() && xpos <= line_coll[j].a.x()) {
                    if (!active) std::cout << "0";
                    active = true;
                }
            }
            if (!active) std::cout << "'";
        }
    }
}


void
NozzleLine::calcMinMax() {
    float x_min;
    float x_max;
    if (line_coll.size() != 0) {
        x_min = line_coll[0].a.x();
        x_max = line_coll[0].a.x();
        for (int k(0); k < line_coll.size(); k++) {
            Line2D line = line_coll[k];
            if (line.a.x() < x_min) x_min = line.a.x();
            if (line.a.x() > x_max) x_max = line.a.x();
            if (line.b.x() < x_min) x_min = line.b.x();
            if (line.b.x() > x_max) x_max = line.b.x();
        }
#ifdef DEBUG_SWEEP
        std::cout << "Xmin : " << xmin << " | Xmax : " << xmax << std::endl;
#endif
        xmin = x_min;
        xmax = x_max;
    }
}


/************************/
/*	   NozzleAction 	*/
/*						*/
/************************/

std::string NozzleAction::toGcode(bool dir) {
    std::string out = "G1 X";
    out += (dir) ? ("-") : ("");
    out += std::to_string(length);
    out += " E";
    out += std::to_string(length/(dpiconst));
    out += " S" + std::to_string(code);
    out += "\n";
    return out;
}

/************************/
/*	       Sweep 		*/
/*						*/
/************************/

void Sweep::drawLines() {
    for (int i = 0; i < nozzleLines.size(); i++) {
        nozzleLines[i].drawLine();
        std::cout << std::endl;
    }
}

void
Sweep::calcXMinMax() {
    float x_min;
    float x_max;
    if (nozzleLines.size() != 0) {
        if (nozzleLines[0].size() != 0) {
            x_min = nozzleLines[0].get(0).a.x();
            x_max = nozzleLines[0].get(0).a.x();
            for (int j(0); j < nozzleLines.size(); j++) {
                NozzleLine curNLine = nozzleLines[j]; //Collection of line on the same Y
                for (int k(0); k < curNLine.size(); k++) {
                    Line2D line = curNLine.get(k);
                    if (line.a.x() < x_min) x_min = line.a.x();
                    if (line.a.x() > x_max) x_max = line.a.x();
                    if (line.b.x() < x_min) x_min = line.b.x();
                    if (line.b.x() > x_max) x_max = line.b.x();
                }
            }
            xmin = x_min;
            xmax = x_max;
        }
    }
}


void
Sweep::calcYMinMax() {
    float y_min;
    float y_max;
    if (nozzleLines.size() != 0) {
        if (nozzleLines[0].size() != 0) {
            y_min = nozzleLines[0].get(0).a.y();
            y_max = nozzleLines[0].get(0).a.y();
            for (int j(0); j < nozzleLines.size(); j++) {
                NozzleLine curNLine = nozzleLines[j]; //Collection of line on the same Y
                for (int k(0); k < curNLine.size(); k++) {
                    Line2D line = curNLine.get(k);
                    if (line.a.y() < y_min) y_min = line.a.y();
                    if (line.a.y() > y_max) y_max = line.a.y();
                    if (line.b.y() < y_min) y_min = line.b.y();
                    if (line.b.y() > y_max) y_max = line.b.y();
                }
            }
            ymin = y_min;
            ymax = y_max;
        }
    }
}


void
Sweep::calcOrigin() {
    origin = Point(xmin, ymin);
    end = Point(xmax, ymin);
}



float
Sweep::getNextX(float boundX) {
    if (nozzleLines.size() != 0) {
        if (nozzleLines[0].size() != 0) {
            float last_min = xmax;

            for (int j(0); j < nozzleLines.size(); j++) {
                NozzleLine curNLine = nozzleLines[j]; //Collection of line on the same Y
                for (int k(0); k < curNLine.size(); k++) {
                    Line2D line = curNLine.get(k);
                    if (line.a.x() < last_min && line.a.x() > boundX) last_min = line.a.x();
                    if (line.b.x() < last_min && line.b.x() > boundX) last_min = line.b.x();
                }
            }
            return last_min;
        }
    }
    return 0;
}


std::vector<bool>
Sweep::getbyX(float X) {
    std::vector<bool> out;
    for (int j(0); j < firstNozzle; j++) {
        out.push_back(false);
    }
    for (int j(firstNozzle); j < nozzleLines.size() + firstNozzle; j++) {
        out.push_back(isNozzleOn(j - firstNozzle, X));
    }
    return out;
}


int
Sweep::arrayToCode(std::vector<bool> nozzleArray) {
    int out = 0;
#ifdef DEBUG_SWEEP
    std::cout << std::endl << "  [";
#endif
    for (int i(firstNozzle); i < nozzleArray.size(); i++) {
        if (nozzleArray[i]) out += pow(2, i);
#ifdef DEBUG_SWEEP
        std::cout << nozzleArray[i] << ((i < nozzleArray.size() - 1) ? ":" : "]");
#endif
    }
#ifdef DEBUG_SWEEP
    std::cout << std::endl << "  -> Out: " << out << std::endl;
#endif
    return out;
}


bool
Sweep::isNozzleOn(int nozzleN, float X) {
    if (nozzleLines.size() != 0) {
        if (nozzleLines[0].size() != 0) {
            if (nozzleN < nozzleLines.size()) {
                NozzleLine curNLine = nozzleLines[nozzleN]; //Collection of line on the same Y
                for (int k(0); k < curNLine.size(); k++) {
                    Line2D line = curNLine.get(k);
                    if (line.a.x() <= X && line.b.x() > X) // -
                        return true;				   //   -
                    if (line.b.x() <= X && line.a.x() > X) //   -
                        return true;				   //	 - Avoid zero length line
                }
            }
        }
    }
    return false;
}


std::string
Sweep::toGcode(bool dir) {
    //std::cout << "Nozzle Action size :" << nozzleActions.size() << std::endl;
    std::string out = "";
    out += absolute();
    out += (dir) ? goToLeftPoint() : goToRightPoint();
    out += ";Sweep\n" + relative();
    if (!dir) {
        for (int i(0); i < nozzleActions.size(); i++) {
            out += nozzleActions[i].toGcode(dir);
        }
    }
    else {
        for (int i(nozzleActions.size() - 1); i > -1; i--) {
            out += nozzleActions[i].toGcode(dir);
        }
    }
    //out += absolute();
    out += ";End Sweep\n\n";
    return out;
}


std::string Sweep::goToRightPoint() {
    return absolute() + "G1 X" + std::to_string(xmin) + " Y" + std::to_string(ymin) + "; Sweep Origin\n";
}

std::string Sweep::goToLeftPoint() {
    return absolute() + "G1 X" + std::to_string(xmax) + " Y" + std::to_string(ymin) + "; Sweep Origin\n";
}



/************************/
/*	  SweepCollection	*/
/*						*/
/************************/

SweepCollection::SweepCollection() {}

std::string
SweepCollection::toGcode(short nPass) {
    std::string out = "";
#ifdef DEBUG_SWEEP
    std::cout << std::endl << "Number of sweep : " << sweeps.size() << std::endl;
#endif
    for (int i(0); i < sweeps.size(); i++) {
        for (int j(0); j < nPass; j++) {
            lastDir = !lastDir;
            out += sweeps[i].toGcode(lastDir);
        }
    }
    return out;
}




SweepCollection
SweepCollection::generateSweeps(std::vector<Line2D> lines, short firstNozzle, short lastNozzle, short dpi) {

    SweepCollection sweeps;

    float spacing = 25.4 / (float)dpi;
    std::vector<Line2D> sortedLines;

    { //Ignore non horizontal lines
        std::vector<Line2D> unsortedLines;
        for (std::vector<Line2D>::const_iterator line = lines.begin(); line != lines.end(); line++) {
            //if (line->a.y == line->b.y)
                unsortedLines.push_back(*line);
        }

        //Sorting lines
#ifdef DEBUG_SWEEP
        std::cout << "Sorting Line...";
#endif
        std::sort(unsortedLines.begin(), unsortedLines.end());
        sortedLines = unsortedLines;
        if (sortedLines.size() == 0) return sweeps; // Is this really neccessary ?
#ifdef DEBUG_SWEEP
        std::cout << "Done" << std::endl;
#endif
    }

    //Generate sweep
#ifdef DEBUG_SWEEP
    std::cout << "Generating Sweep...";
#endif
    float min, max;
    min = getMinY(sortedLines);
    max = getMaxY(sortedLines);
#ifdef DEBUG_SWEEP
    std::cout << std::endl << "Min Max : " << "Min: " << min << " | Max: " << max << std::endl;
#endif
    int index = 0;
    Sweep sweep; //Collection of line of in a sweep
    NozzleLine nozzleLine; //Collection of line with the same height
    float y = min;
    while(y <= max){
        nozzleLine.clear();
        while(sortedLines[index].a.y() < y + (spacing / 2.0) && sortedLines[index].a.y() >= y - (spacing / 2.0)) {
            nozzleLine.push(sortedLines[index]);
            index++;
            if (index >= sortedLines.size()) break;
        }
        if (nozzleLine.size() != 0) sweep.push(nozzleLine);
        else sweep.push(NozzleLine(Line2D(Point(0.0, y), Point(0.0, y))));
        if (sweep.size() == (lastNozzle+1 - firstNozzle)) {
            sweep.setNozzle(firstNozzle, lastNozzle);
            sweeps.push(sweep);
            sweep.clear();
        }
        y += spacing;
    }
#ifdef DEBUG_SWEEP
    std::cout << "Done" << std::endl;
#endif
    if (sweep.size() != 0) sweeps.push(sweep);

    //Generate the sweepAction
#ifdef DEBUG_SWEEP
    std::cout << "Generating Nozzle Action..." << std::endl;
#endif


    for (std::vector<Sweep>::iterator sweep = sweeps.begin(); sweep != sweeps.end(); ++sweep) {

        sweep->calcMinMax();


#ifdef DEBUG_SWEEP
        std::cout << std::endl << "New Sweep : " << "Min: " << sweep->getXMin() << " | Max: " << sweep->getXMax() << std::endl;
#endif
        float curX = sweep->getXMin();
        float nextX = sweep->getNextX(curX);

        int lastCode = 0;
        while (curX != sweep->getXMax()) {
#ifdef DEBUG_SWEEP
            std::cout << std::endl << " curX : " << curX << " | nextX : " << nextX;
#endif
            int code = sweep->arrayToCode(sweep->getbyX(curX));
            if (lastCode != code || sweep->sizeA() == 0) {
                NozzleAction n = NozzleAction(curX, abs(nextX - curX), code, 96);
                sweep->addNozzleAction(n);
                lastCode = code;
            }
            else {
                sweep->addLengthOf(sweep->sizeA() - 1 ,abs(nextX - curX));
            }

            curX = sweep->getNextX(curX);
            nextX = sweep->getNextX(curX);
        }
    }

#ifdef DEBUG_SWEEP
    std::cout << std::endl << "-----------------------------" << std::endl;
    std::cout << "    2D Result :" << std::endl << std::endl;
    for(int i=0; i < sweeps.size();i++)
        sweeps.get(i).drawLines();

#endif

    return sweeps;
}
