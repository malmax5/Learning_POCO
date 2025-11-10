#include "Poco/Random.h"
#include "Poco/Types.h"
#include <iostream>

int main()
{
    Poco::Random rngSeed;
    Poco::Random rngInputGeneratior;
    rngSeed.seed(12345); // Сидирование случайным числом - сидом
    rngInputGeneratior.seed(); // RandomInputGeneratior для получения энтропии

    Poco::UInt32 randomInt = rngInputGeneratior.next();
    Poco::UInt32 diceRoll = rngInputGeneratior.next(6 + 1);

    char randomChar = rngInputGeneratior.nextChar();
    bool randomBool = rngInputGeneratior.nextBool();
    float randomFloat = rngInputGeneratior.nextFloat();
    double randomDouble = rngInputGeneratior.nextDouble();

    std::cout << "Random int: " << randomInt << "\n";
    std::cout << "Random int in [0, 7): " << randomInt << "\n";
    std::cout << "Random char: " << randomChar << "\n";
    std::cout << "Random bool: " << randomBool << "\n";
    std::cout << "Random float: " << randomFloat << "\n";
    std::cout << "Random double: " << randomDouble << "\n";
}
