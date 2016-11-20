#include "pnpparser.h"
#include "tools/csvparser.h"
#include "tools/stringoperations.h"

PnpParser::PnpParser()
{

}

bool PnpParser::parse(istream &inStream, Components &outComponents){

    CSVRow row;
    bool status = true;

    //header
    mHeader.clear();
    inStream >> row;
    for (uint32_t i = 0; i < row.size(); i ++){
        mHeader.push_back(row[i]);
    }

    // blank line
    string dummy;
    getline(inStream, dummy);


    string des;
    map <string, string> params;
    while(inStream >> row){
        for (uint32_t i = 0; i < mHeader.size(); i ++){
            if(mHeader[i].compare(mDesString) == 0){
                des = row[i];
            }
            else{
                params[mHeader[i]] = row[i];
            }
        }


        // the component doesn't exist yet
        Components::iterator it_oc = outComponents.find(des);
        if(it_oc == outComponents.end()){
            outComponents[des] = Component();
            Component &compo = outComponents[des];
            compo.setDesignator(des);
        }

        Component &compo = outComponents[des];
        double x=0.0, y=0.0;
        for(map <string, string>::iterator rcpi = params.begin(); rcpi != params.end(); ++rcpi){
            string name = rcpi->first;
            string value = rcpi->second;

            if(name.compare(mDesString) == 0){
                // nothing to do
            }
            else if (name.compare(mXString) == 0 || name.compare(mYString) == 0){
                if(value.find("mm") != string::npos && compo.getUnit() != Component::eUnitMil){
                    compo.setUnit(Component::eUnitMm);
                    replace(value, "mm", "");
                }
                else if(value.find("mil") != string::npos && compo.getUnit() != Component::eUnitMm){
                    compo.setUnit(Component::eUnitMil);
                    replace(value, "mil", "");
                }
                else{
                    compo.setUnit(Component::eUnitNotSet);
                }

                double coord = 0;
                try{ coord = stringToDouble(value); }
                catch(...){
                    err_printf("ERROR(PnpParser::parse): parsing coord X/Y");
                    status = false;
                }

                if(name.compare(mXString) == 0) {
                    x = coord;
                }
                else {
                    y = coord;
                }
            }

            else if (name.compare(mRotString) == 0){
                try{
                    double rot = stringToDouble(value);
                    compo.setRotation(rot);
                }
                catch(...){
                    err_printf("ERROR(PnpParser::parse): parsing rot");
                    status = false;
                }
            }
            else{
                Component::Parameters &params = compo.getParameters();
                Component::Parameters::iterator it = params.find((name));
                if(it == params.end()){
                    compo.getParameters()[name] = value;
                }
            }
        }

        compo.setPosition(plPoint(x,y));
    }

    return status;
}
