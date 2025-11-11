#include "Poco/PriorityEvent.h"
#include "Poco/PriorityDelegate.h"
#include "Poco/Expire.h"
#include "Poco/Timestamp.h"
#include <iostream>

// Событие заказа
class OrderEvent
{
public:
    OrderEvent(int orderId, const std::string& product, int quantity)
        : _orderId(orderId), _product(product), _quantity(quantity), 
          _timestamp(Poco::Timestamp())
    {

    }
    
    int getOrderId() const
    {
        return _orderId;
    }

    std::string getProduct() const
    {
        return _product;
    }

    int getQuantity() const
    {
        return _quantity;
    }

    Poco::Timestamp getTimestamp() const
    {
        return _timestamp;
    }
    
    // Позволяем обработчикам модифицировать заказ
    void setQuantity(int qty)
    {
        _quantity = qty;
    }

    
private:
    int _orderId;
    std::string _product;
    int _quantity;
    Poco::Timestamp _timestamp;
};

// Система обработки заказов
class OrderSystem
{
public:
    Poco::PriorityEvent<OrderEvent&> orderReceived; // Событие с приоритетом
    
    void placeOrder(int orderId, const std::string& product, int quantity, int priority = 0)
    {
        OrderEvent order(orderId, product, quantity);
        std::cout << "Placing order #" << orderId << " with priority " << priority << "\n";
        orderReceived.notify(this, order);
        
        std::cout << "Final processed quantity: " << order.getQuantity() << "\n";
    }
};

// Обработчики заказов
class InventoryManager
{
public:
    void checkStock(const void* pSender, OrderEvent& order)
    {
        std::cout << "  [Inventory] Checking stock for: " << order.getProduct() 
                  << " (Qty: " << order.getQuantity() << ")\n";
        
        // Можем модифицировать заказ
        if (order.getQuantity() > 100) {
            std::cout << "  [Inventory] Reducing quantity (was " << order.getQuantity() << ")\n";
            order.setQuantity(100); // Ограничение запасов
        }
    }
};

class PaymentProcessor
{
public:
    void processPayment(const void* pSender, OrderEvent& order)
    {
        std::cout << "  [Payment] Processing payment for order #" 
                  << order.getOrderId() << "\n";
    }
};

// Временный обработчик для акции
class PromotionHandler
{
public:
    void applyPromotion(const void* pSender, OrderEvent& order)
    {
        std::cout << "  [Promotion] Applying special discount!\n";
    }
};

int main()
{
    OrderSystem system;
    InventoryManager inventory;
    PaymentProcessor payment;
    PromotionHandler promotion;
    
    // Постоянные обработчики с разными приоритетами
    system.orderReceived += Poco::PriorityDelegate<InventoryManager, OrderEvent&>(
        &inventory, &InventoryManager::checkStock, 10);
        
    system.orderReceived += Poco::PriorityDelegate<PaymentProcessor, OrderEvent&>(
        &payment, &PaymentProcessor::processPayment, 0);
    
    // Временный обработчик (автоматически отпишется через 5 секунд) -> не работает, возможно в новой версии работает
    // auto promotionDelegate = PriorityDelegate<PromotionHandler, OrderEvent&>(
    //     &promotion, &PromotionHandler::applyPromotion, 5);
    // system.orderReceived += Expire<OrderEvent&>(promotionDelegate, 5000); // Средний приоритет
    
    std::cout << "=== Processing orders ===" << "\n";
    system.placeOrder(1, "Laptop", 2, 1);
    std::cout << "---" << "\n";
    system.placeOrder(2, "Smartphone", 150, 1); // Большой заказ
    
    std::cout << "\nWaiting 6 seconds for promotion to expire..." << "\n";
    Poco::Thread::sleep(6000);
    
    std::cout << "\n=== After promotion expired ===" << "\n";
    system.placeOrder(3, "Tablet", 5, 1);
    
    return 0;
}