///
/// \file    cafeRoast.h
/// \author  Jan Chrin, PSI
/// \date    Release: January 2016
/// \version CAFE 1.3.0
///
///  Add Match methods
///

#ifndef CAFEROAST_H
#define CAFEROAST_H


int setAndGet (const unsigned int handleSet, dbr_double_t valSet, dbr_double_t &valGet)
{
    status=cafeDoppio.setAndGet((unsigned int) handleSet, DBR_DOUBLE, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_float_t valSet, dbr_float_t &valGet)
{
    status=cafeFrappuccino.setAndGet((unsigned int) handleSet, DBR_FLOAT, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_char_t valSet, dbr_char_t &valGet)
{
    status=cafeCappuccino.setAndGet((unsigned int) handleSet, DBR_CHAR, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_enum_t valSet, dbr_enum_t &valGet)
{
    status=cafeEspresso.setAndGet((unsigned int) handleSet, DBR_ENUM, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_short_t valSet, dbr_short_t &valGet)
{
    status=cafeSchale.setAndGet((unsigned int) handleSet, DBR_SHORT, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_long_t valSet, dbr_long_t &valGet)
{
    status=cafeLatte.setAndGet((unsigned int) handleSet, DBR_LONG, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, long long valSet, long long &valGet)
{
    double valSetD=valSet;
    double valGetD=0;
    status=cafeDoppio.setAndGet((unsigned int) handleSet, DBR_DOUBLE, valSetD, valGetD);
    valGet=(long long) valGetD;
    return status;
}
int setAndGet (const unsigned int handleSet, dbr_string_t valSet, dbr_string_t &valGet)
{
    status=cafeSoluble.setAndGetDbrString((unsigned int) handleSet, valSet, valGet);
    return status;
}
int setAndGet (const unsigned int handleSet, std::string valSet, std::string &valGet)
{
    status=cafeSoluble.setAndGetString((unsigned int) handleSet, valSet, valGet);
    return status;
}

//
int match(dbr_double_t valSet, unsigned int handleMatch,
          dbr_double_t tolerance, double timeout, bool printFlag)
{
    return cafeDoppio.match(DBR_DOUBLE, valSet, handleMatch, tolerance, timeout, printFlag);
}


int match(dbr_float_t valSet, unsigned int handleMatch,
          dbr_float_t tolerance, double timeout, bool printFlag)
{
    return cafeFrappuccino.match(DBR_FLOAT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int match(dbr_char_t valSet, unsigned int handleMatch,
          dbr_char_t tolerance, double timeout, bool printFlag)
{
    return cafeCappuccino.match(DBR_CHAR, valSet, handleMatch, tolerance, timeout, printFlag);
}

int match(dbr_enum_t valSet, unsigned int handleMatch,
          dbr_enum_t tolerance, double timeout, bool printFlag)
{
    return cafeEspresso.match(DBR_ENUM, valSet, handleMatch, tolerance, timeout, printFlag);
}

int match(dbr_short_t valSet, unsigned int handleMatch,
          dbr_short_t tolerance, double timeout, bool printFlag)
{
    return cafeSchale.match(DBR_SHORT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int match(dbr_long_t valSet, unsigned int handleMatch,
          dbr_long_t tolerance, double timeout, bool printFlag)
{
    return cafeLatte.match(DBR_LONG, valSet, handleMatch, tolerance, timeout, printFlag);
}


int match(long long valSet, unsigned int handleMatch,
          long long tolerance, double timeout, bool printFlag)
{
    dbr_double_t valSetDouble=valSet;
    dbr_double_t toleranceDouble = tolerance;
    return cafeDoppio.match(DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
}

int match(std::string valSet, unsigned int handleMatch,
          std::string tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "match (string valSet, unsigned int handleMatch,\
                string tolerance, double timeout, bool printFlag)"

    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t valSetDouble=0;
    dbr_double_t toleranceDouble = 0;
    ss.clear();
    ss.str(valSet);
    ss>>d;

    if ( !ss.fail()) {
        valSetDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << valSet;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }
    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.match(DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
#undef __METHOD__
}



int match(dbr_string_t valSet, unsigned int handleMatch,
          dbr_string_t tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "match (dbr_string_t valSet, unsigned int handleMatch,\
                dbr_string_t tolerance, double timeout, bool printFlag)"
    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t valSetDouble=0;
    dbr_double_t toleranceDouble = 0;
    ss.clear();
    ss.str(valSet);
    ss>>d;

    if ( !ss.fail()) {
        valSetDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << valSet;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }
    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.match(DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
#undef __METHOD__
}

//
int matchMany(std::vector<dbr_double_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_double_t tolerance, double timeout, bool printFlag)
{
    return cafeDoppio.matchMany(DBR_DOUBLE, valSet, handleMatch, tolerance, timeout, printFlag);
}


int matchMany(std::vector<dbr_float_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_float_t tolerance, double timeout, bool printFlag)
{
    return cafeFrappuccino.matchMany(DBR_FLOAT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int matchMany(std::vector<dbr_char_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_char_t tolerance, double timeout, bool printFlag)
{
    return cafeCappuccino.matchMany(DBR_CHAR, valSet, handleMatch, tolerance, timeout, printFlag);
}

int matchMany(std::vector<dbr_enum_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_enum_t tolerance, double timeout, bool printFlag)
{
    return cafeEspresso.matchMany(DBR_ENUM, valSet, handleMatch, tolerance, timeout, printFlag);
}

int matchMany(std::vector<dbr_short_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_short_t tolerance, double timeout, bool printFlag)
{
    return cafeSchale.matchMany(DBR_SHORT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int matchMany(std::vector<dbr_long_t> valSet, std::vector<unsigned int> handleMatch,
              dbr_long_t tolerance, double timeout, bool printFlag)
{
    return cafeLatte.matchMany(DBR_LONG, valSet, handleMatch, tolerance, timeout, printFlag);
}


int matchMany(std::vector<long long> valSet, std::vector<unsigned int> handleMatch,
              long long tolerance, double timeout, bool printFlag)
{

    std::vector<dbr_double_t> valSetDoubleV;
    valSetDoubleV.reserve(valSet.size());

    std::copy(valSet.begin(), valSet.end(),
              std::back_inserter(valSetDoubleV));

    dbr_double_t toleranceDouble = tolerance;

    return cafeDoppio.matchMany(DBR_DOUBLE, valSetDoubleV, handleMatch, toleranceDouble, timeout, printFlag);
}

int matchMany(std::vector<std::string> valSetV, std::vector<unsigned int> handleMatchV,
              std::string tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "matchMany (std::vector<string> valSetV, std::vector<unsigned int> handleMatchV, \
                string tolerance, double timeout, bool printFlag)"

    std::vector<dbr_double_t> valSetDoubleV;

    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t toleranceDouble = 0;
    for (size_t i=0; i< valSetV.size(); ++i) {
        d=0;
        ss.clear();
        ss.str(valSetV[i]);
        ss>>d;

        if ( !ss.fail()) {
            valSetDoubleV.push_back(d);
        }
        else {
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
            std::cout << "***WARNING*** COULD NOT CONVERT: ";
            std::cout << valSetV[i];
            std::cout << " TO DOUBLE!" << std::endl;
            return ECAFE_NO_CONVERT;
        }
    }

    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.matchMany(DBR_DOUBLE, valSetDoubleV, handleMatchV, toleranceDouble, timeout, printFlag);
#undef __METHOD__
}




int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_double_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_double_t tolerance, double timeout, bool printFlag)
{
    return cafeDoppio.setAndMatchMany(handleSet, DBR_DOUBLE, valSet, handleMatch, tolerance, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_float_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_float_t tolerance, double timeout, bool printFlag)
{
    return cafeFrappuccino.setAndMatchMany(handleSet, DBR_FLOAT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_char_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_char_t tolerance, double timeout, bool printFlag)
{
    return cafeCappuccino.setAndMatchMany(handleSet, DBR_CHAR, valSet, handleMatch, tolerance, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_enum_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_enum_t tolerance, double timeout, bool printFlag)
{
    return cafeEspresso.setAndMatchMany(handleSet, DBR_ENUM, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_short_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_short_t tolerance, double timeout, bool printFlag)
{
    return cafeSchale.setAndMatchMany(handleSet, DBR_SHORT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<dbr_long_t> valSet, std::vector<unsigned int> handleMatch,
                    dbr_long_t tolerance, double timeout, bool printFlag)
{
    return cafeLatte.setAndMatchMany(handleSet, DBR_LONG, valSet, handleMatch, tolerance, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSet, std::vector<long long> valSet, std::vector<unsigned int> handleMatch,
                    long long tolerance, double timeout, bool printFlag)
{

    std::vector<dbr_double_t> valSetDoubleV;
    valSetDoubleV.reserve(valSet.size());

    std::copy(valSet.begin(), valSet.end(),
              std::back_inserter(valSetDoubleV));

    dbr_double_t toleranceDouble = tolerance;

    return cafeDoppio.setAndMatchMany(handleSet, DBR_DOUBLE, valSetDoubleV, handleMatch, toleranceDouble, timeout, printFlag);
}


int setAndMatchMany(std::vector<unsigned int> handleSetV, std::vector<std::string> valSetV, std::vector<unsigned int> handleMatchV,
                    std::string tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "setAndMatchMany (std::vector<unsigned int> handleSetV, std::vector<string> valSetV, std::vector<unsigned int> handleMatchV, \
                string tolerance, double timeout, bool printFlag)"

    std::vector<dbr_double_t> valSetDoubleV;

    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t toleranceDouble = 0;
    for (size_t i=0; i< valSetV.size(); ++i) {
        d=0;
        ss.clear();
        ss.str(valSetV[i]);
        ss>>d;

        if ( !ss.fail()) {
            valSetDoubleV.push_back(d);
        }
        else {
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
            std::cout << "***WARNING*** COULD NOT CONVERT: ";
            std::cout << valSetV[i];
            std::cout << " TO DOUBLE!" << std::endl;
            return ECAFE_NO_CONVERT;
        }
    }

    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.setAndMatchMany(handleSetV, DBR_DOUBLE, valSetDoubleV, handleMatchV, toleranceDouble, timeout, printFlag);
#undef __METHOD__

}





int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_double_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_double_t tolerance, double timeout, bool printFlag)
{
    cafeDoppio.setMany   (handleSet,    DBR_DOUBLE, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeDoppio.compareAndMatchMany(handleSet, DBR_DOUBLE, valSet, handleMatch, tolerance, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_float_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_float_t tolerance, double timeout, bool printFlag)
{
    cafeFrappuccino.setMany   (handleSet,    DBR_FLOAT, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeFrappuccino.compareAndMatchMany(handleSet, DBR_FLOAT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_char_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_char_t tolerance, double timeout, bool printFlag)
{
    cafeCappuccino.setMany   (handleSet,    DBR_CHAR, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeCappuccino.compareAndMatchMany(handleSet, DBR_CHAR, valSet, handleMatch, tolerance, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_enum_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_enum_t tolerance, double timeout, bool printFlag)
{
    cafeEspresso.setMany   (handleSet,    DBR_ENUM, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeEspresso.compareAndMatchMany(handleSet, DBR_ENUM, valSet, handleMatch, tolerance, timeout, printFlag);
}

int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_short_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_short_t tolerance, double timeout, bool printFlag)
{
    cafeSchale.setMany   (handleSet,    DBR_SHORT, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeSchale.compareAndMatchMany(handleSet, DBR_SHORT, valSet, handleMatch, tolerance, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<dbr_long_t> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    dbr_long_t tolerance, double timeout, bool printFlag)
{
    cafeLatte.setMany   (handleSet,    DBR_LONG, valSet, false);
    cafeSoluble.setManyString  (handleAction, valAction, false);
    return cafeLatte.compareAndMatchMany(handleSet, DBR_LONG, valSet, handleMatch, tolerance, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSet, std::vector<long long> valSet,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                    long long tolerance, double timeout, bool printFlag)
{

    std::vector<dbr_double_t> valSetDoubleV;
    valSetDoubleV.reserve(valSet.size());

    std::copy(valSet.begin(), valSet.end(),
              std::back_inserter(valSetDoubleV));

    dbr_double_t toleranceDouble = tolerance;

    return gameSetAndMatch(handleSet, valSetDoubleV,
                           handleAction, valAction,  handleMatch, toleranceDouble, timeout, printFlag);
}


int gameSetAndMatch(std::vector<unsigned int> handleSetV, std::vector<std::string> valSetV,
                    std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatchV,
                    std::string tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "gameSetAndMatch (std::vector<unsigned int> handleSetV, std::vector<string> valSetV, \
                std::vector<unsigned int> handleAction, std::vector<string> valAction, std::vector<unsigned int> handleMatchV, \
                string tolerance, double timeout, bool printFlag)"

    std::vector<dbr_double_t> valSetDoubleV;

    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t toleranceDouble = 0;
    for (size_t i=0; i< valSetV.size(); ++i) {
        d=0;
        ss.clear();
        ss.str(valSetV[i]);
        ss>>d;

        if ( !ss.fail()) {
            valSetDoubleV.push_back(d);
        }
        else {
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
            std::cout << "***WARNING*** COULD NOT CONVERT: ";
            std::cout << valSetV[i];
            std::cout << " TO DOUBLE!" << std::endl;
            return ECAFE_NO_CONVERT;
        }
    }

    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return gameSetAndMatch(handleSetV, valSetDoubleV,  handleAction, valAction, handleMatchV, toleranceDouble, timeout, printFlag);
#undef __METHOD__

}







int setAndMatch(const unsigned int handleSet, dbr_double_t valSet, const unsigned int handleMatch,
                dbr_double_t tolerance, double timeout, bool printFlag)
{
    return cafeDoppio.setAndMatch(handleSet, DBR_DOUBLE, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatch(const unsigned int handleSet, dbr_float_t valSet, const unsigned int handleMatch,
                dbr_float_t tolerance, double timeout, bool printFlag)
{
    return cafeFrappuccino.setAndMatch(handleSet, DBR_FLOAT, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatch(const unsigned int handleSet, dbr_char_t valSet, const unsigned int handleMatch,
                dbr_char_t tolerance, double timeout, bool printFlag)
{
    return cafeCappuccino.setAndMatch(handleSet, DBR_CHAR, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatch(const unsigned int handleSet, dbr_enum_t valSet, const unsigned int handleMatch,
                dbr_enum_t tolerance, double timeout, bool printFlag)
{
    return cafeEspresso.setAndMatch(handleSet, DBR_ENUM, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatch(const unsigned int handleSet, dbr_short_t valSet, const unsigned int handleMatch,
                dbr_short_t tolerance, double timeout, bool printFlag)
{
    return cafeSchale.setAndMatch(handleSet, DBR_SHORT, valSet, handleMatch, tolerance, timeout, printFlag);
}

int setAndMatch(const unsigned int handleSet, dbr_long_t valSet, const unsigned int handleMatch,
                dbr_long_t tolerance, double timeout, bool printFlag)
{
    return cafeLatte.setAndMatch(handleSet, DBR_LONG, valSet, handleMatch, tolerance, timeout, printFlag);
}
int setAndMatch(const unsigned int handleSet, long long valSet, const unsigned int handleMatch,
                long long tolerance, double timeout, bool printFlag)
{
    dbr_double_t valSetDouble=valSet;
    dbr_double_t toleranceDouble = tolerance;
    return cafeDoppio.setAndMatch(handleSet, DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
}


int setAndMatch(const unsigned int handleSet, dbr_string_t valSet, const unsigned int handleMatch,
                dbr_string_t tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "setAndMatch (dbr_string_t input)"
    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t valSetDouble=0;
    dbr_double_t toleranceDouble = 0;
    ss.clear();
    ss.str(valSet);
    ss>>d;

    if ( !ss.fail()) {
        valSetDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << valSet;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }
    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.setAndMatch(handleSet, DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
#undef __METHOD__
}

int setAndMatch(const unsigned int handleSet, std::string valSet, const unsigned int handleMatch,
                std::string tolerance, double timeout, bool printFlag)
{
#define __METHOD__ "setAndMatch (string input)"
    std::istringstream ss;
    dbr_double_t d=0;
    dbr_double_t valSetDouble=0;
    dbr_double_t toleranceDouble = 0;
    ss.clear();
    ss.str(valSet);
    ss>>d;

    if ( !ss.fail()) {
        valSetDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << valSet;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }
    d=0;
    ss.clear();
    ss.str(tolerance);
    ss>>d;

    if ( !ss.fail()) {
        toleranceDouble=d;
    }
    else {
        std::cout << __METHOD__ << __LINE__ << std::endl;
        std::cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION " << std::endl;
        std::cout << "***WARNING*** COULD NOT CONVERT: ";
        std::cout << tolerance;
        std::cout << " TO DOUBLE!" << std::endl;
        return ECAFE_NO_CONVERT;
    }

    return cafeDoppio.setAndMatch(handleSet, DBR_DOUBLE, valSetDouble, handleMatch, toleranceDouble, timeout, printFlag);
#undef __METHOD__
}

#endif // CAFEROAST_H
