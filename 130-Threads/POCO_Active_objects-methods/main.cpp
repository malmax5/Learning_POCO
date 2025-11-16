#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveResult.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveStarter.h"
#include "Poco/Activity.h"
#include <iostream>
#include <vector>
#include <future>

using Poco::Runnable;
using Poco::Thread;
using Poco::Mutex;
using Poco::ScopedLock;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::ActiveDispatcher;
using Poco::ActiveStarter;
using Poco::Activity;
using namespace std;

// 1. ActiveMethod - асинхронные методы
class AsyncCalculator : public ActiveDispatcher
{
public:
    // Асинхронные методы
    ActiveMethod<int, pair<int, int>, AsyncCalculator, 
                 ActiveStarter<ActiveDispatcher>> add;
    
    ActiveMethod<double, pair<double, double>, AsyncCalculator,
                 ActiveStarter<ActiveDispatcher>> multiply;
    
    ActiveMethod<string, string, AsyncCalculator,
                 ActiveStarter<ActiveDispatcher>> processString;
    
    AsyncCalculator() : 
        add(this, &AsyncCalculator::addImpl),
        multiply(this, &AsyncCalculator::multiplyImpl),
        processString(this, &AsyncCalculator::processStringImpl)
    {

    }
    
private:
    int addImpl(const pair<int, int>& args)
    {
        cout << "Adding " << args.first << " + " << args.second << endl;
        Thread::sleep(100); // Имитация работы
        return args.first + args.second;
    }
    
    double multiplyImpl(const pair<double, double>& args)
    {
        cout << "Multiplying " << args.first << " * " << args.second << endl;
        Thread::sleep(150);
        return args.first * args.second;
    }
    
    string processStringImpl(const string& input)
    {
        cout << "Processing string: " << input << endl;
        Thread::sleep(200);
        string result = "Processed: " + input;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

void active_method_demo()
{
    cout << "=== Active Method Demo ===" << endl;
    
    AsyncCalculator calculator;
    
    // Запускаем несколько асинхронных операций
    vector<ActiveResult<int>> addResults;
    vector<ActiveResult<double>> multiplyResults;
    vector<ActiveResult<string>> stringResults;
    
    // Асинхронные вызовы
    for (int i = 0; i < 3; ++i)
    {
        addResults.push_back(calculator.add(make_pair(i * 10, i * 5)));
        multiplyResults.push_back(calculator.multiply(make_pair(i * 1.5, i * 2.0)));
        stringResults.push_back(calculator.processString("test" + to_string(i)));
    }
    
    // Получаем результаты
    cout << "\n--- Addition Results ---" << endl;
    for (auto& result : addResults)
    {
        result.wait();
        cout << "Result: " << result.data() << endl;
    }
    
    cout << "\n--- Multiplication Results ---" << endl;
    for (auto& result : multiplyResults)
    {
        result.wait();
        cout << "Result: " << result.data() << endl;
    }
    
    cout << "\n--- String Processing Results ---" << endl;
    for (auto& result : stringResults)
    {
        result.wait();
        cout << "Result: " << result.data() << endl;
    }
}

// 2. Activity - длительные операции
class DataProcessor
{
private:
    Activity<DataProcessor> _activity;
    vector<int> _data;
    atomic<bool> _processing;
    mutable Mutex _dataMutex;
    
public:
    DataProcessor() 
        : _activity(this, &DataProcessor::processData), _processing(false)
    {
        // Инициализируем тестовые данные
        for (int i = 0; i < 100; ++i)
        {
            _data.push_back(i);
        }
    }
    
    ~DataProcessor()
    {
        stop();
    }
    
    void start()
    {
        if (!_processing)
        {
            _processing = true;
            _activity.start();
            cout << "Data processing started" << endl;
        }
    }
    
    void stop()
    {
        if (_processing)
        {
            _processing = false;
            _activity.stop();
            _activity.wait();
            cout << "Data processing stopped" << endl;
        }
    }
    
    void addData(int value)
    {
        ScopedLock<Mutex> lock(_dataMutex);
        _data.push_back(value);
        cout << "Added data: " << value << endl;
    }
    
    size_t getDataSize() const
    {
        ScopedLock<Mutex> lock(_dataMutex);
        return _data.size();
    }
    
private:
    void processData()
    {
        int processed = 0;
        while (!_activity.isStopped() && _processing)
        {
            vector<int> batch;
            
            // Забираем пачку данных для обработки
            {
                ScopedLock<Mutex> lock(_dataMutex);
                if (!_data.empty())
                {
                    size_t batchSize = min(_data.size(), size_t(10));
                    batch.insert(batch.end(), _data.begin(), _data.begin() + batchSize);
                    _data.erase(_data.begin(), _data.begin() + batchSize);
                }
            }
            
            // Обрабатываем пачку
            if (!batch.empty())
            {
                cout << "Processing batch of " << batch.size() << " items" << endl;
                for (int value : batch)
                {
                    // Имитация обработки
                    Thread::sleep(10);
                    processed++;
                }
                cout << "Processed " << processed << " items total" << endl;
            }
            else
            {
                // Нет данных - ждем
                Thread::sleep(100);
            }
            
            // Периодическая проверка остановки
            if (_activity.isStopped())
            {
                break;
            }
        }
        
        cout << "Data processing completed. Total processed: " << processed << endl;
    }
};

void activity_demo()
{
    cout << "\n=== Activity Demo ===" << endl;
    
    DataProcessor processor;
    
    cout << "Starting data processor..." << endl;
    processor.start();
    
    // Добавляем данные во время работы
    for (int i = 0; i < 5; ++i)
    {
        Thread::sleep(300);
        for (int j = 0; j < 15; ++j)
        {
            processor.addData(i * 100 + j);
        }

        cout << "Data size: " << processor.getDataSize() << endl;
    }
    
    // Даем время на обработку
    Thread::sleep(2000);
    
    cout << "Stopping data processor..." << endl;
    processor.stop();
    
    cout << "Final data size: " << processor.getDataSize() << endl;
}

int main()
{
    active_method_demo();
    activity_demo();
}
