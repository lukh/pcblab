#include "bomparser.h"
#include "tools/csvparser.h"

BomParser::BomParser()
{

}

bool BomParser::parse(istream &inStream, Components &outComponents){

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
        for (int i = 0; i < mHeader.size(); i ++){
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
        for(map <string, string>::iterator rcpi = params.begin(); rcpi != params.end(); ++rcpi){
            string name = rcpi->first;
            string value = rcpi->second;

            if(name.compare(mDesString) == 0){
                // nothing to do
            }
            else{
                Component::Parameters &params = compo.getParameters();
                Component::Parameters::iterator it = params.find((name));
                if(it == params.end()){
                    compo.getParameters()[name] = value;
                }
            }

        }
    }

    return status;
}


