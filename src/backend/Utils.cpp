int getTileFeature(int tileValue){
    return extractBits(tileValue,Constants.tileTypeFeatureStart, Constants.tileTypeFeatureEnd);
}
int getTileColor(int tileValue){
    return extractBits(tileValue,Constants.tileTypeColorStart, Constants.tileColorFeatureEnd);
}

bool tileBlockedMoveUp(int tileValue){
    return extractBits(tileValue, Constants.tileTypeBlockedMoveUp,Constants.tileTypeBlockedMoveUp);
}

bool tileBlockedMoveDown(int tileValue){
    return extractBits(tileValue, Constants.tileTypeBlockedMoveDown,Constants.tileTypeBlockedMoveDown);
}

bool tileBlockedMoveLeft(int tileValue){
    return extractBits(tileValue, Constants.tileTypeBlockedMoveLeft,Constants.tileTypeBlockedMoveLeft);
}

bool tileBlockedMoveRight(int tileValue){
    return extractBits(tileValue, Constants.tileTypeBlockedMoveRight,Constants.tileTypeBlockedMoveRight);
}