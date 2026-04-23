extern int bgIndex;

void setBackgroundFromIndex(int index){

    switch(index){
        case 0: bgIndex = 0; break;
        case 1: bgIndex = 1; break;
        case 2: bgIndex = 2; break;
        case 3: bgIndex = 3; break;
    }
}

char* getNameFromBackgroundIndex(){
    switch(bgIndex){
        case 0: return "BLISS";
        case 1: return "WIN10";
        case 2: return "MINTY";
        case 3: return "MOUTAINS";
        default: return "UNDEFINED";
    }
}