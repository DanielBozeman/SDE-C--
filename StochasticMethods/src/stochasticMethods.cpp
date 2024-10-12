#include "stochasticMethods.hpp"
#include <vector>
#include "RandomUtils.hpp"

std::vector<double> eulerMaruyama(stochastic_function alphaFunction, stochastic_function betaFunction, double initialValue, std::vector<double> timeInterval, std::vector<double> parameters, std::vector<double> brownianPath)
{
    std::vector<double> approximation = {initialValue};

    double dt = timeInterval[1] - timeInterval[0];

    if(brownianPath.size() == 0){
        randomPathMaker rp = randomPathMaker();
        brownianPath = rp.makePath(timeInterval[0], timeInterval.back(), dt);
    }

    approximation.reserve((timeInterval.back() - timeInterval[0])/dt);

    for(int i = 1; i < timeInterval.size(); i++){
        double prevValue = approximation[i-1];
        double prevTime = timeInterval[i-1];
        double dW = brownianPath[i] - brownianPath[i-1];
        approximation.push_back((prevValue + alphaFunction(prevValue, prevTime, parameters)*dt + betaFunction(prevValue, prevTime, parameters)*dW ));
    }

    return approximation;
}

std::vector<std::vector<double>> multipleEulerMaruyama(stochastic_function alphaFunction, stochastic_function betaFunction, double initialValue, std::vector<double> timeInterval, std::vector<std::vector<double>> brownianPaths, std::vector<double> parameters, int numSimulations){
    std::vector<std::vector<double>> approximations;

    double dt = timeInterval[1] - timeInterval[0];

    for(int i = 0; i < numSimulations; i++){
        std::vector<double> approximation = {initialValue};

        approximation.reserve((timeInterval.back() - timeInterval[0])/dt);

        for(int j = 1; j < timeInterval.size(); j++){
            double prevValue = approximation[j-1];
            double prevTime = timeInterval[j-1];
            double dW = brownianPaths[i][j] - brownianPaths[i][j-1];
            approximation.push_back((prevValue + alphaFunction(prevValue, prevTime, parameters)*dt + betaFunction(prevValue, prevTime, parameters)*dW ));
        }

        approximations.push_back(approximation);
    }

    return approximations;
}
