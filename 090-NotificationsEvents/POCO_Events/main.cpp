// События: Система подписки на новости
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/SharedPtr.h"
#include <iostream>
#include <vector>

// Аргумент события - новость
class NewsArticle
{
public:
    NewsArticle(const std::string& title, const std::string& content, const std::string& category)
        : _title(title), _content(content), _category(category)
    {

    }
    
    std::string getTitle() const
    {
        return _title;
    }

    std::string getContent() const
    {
        return _content;
    }

    std::string getCategory() const
    {
        return _category;
    }
    
private:
    std::string _title;
    std::string _content;
    std::string _category;
};

// Источник новостей
class NewsPublisher
{
public:
    Poco::BasicEvent<const NewsArticle&> newsPublished;
    Poco::BasicEvent<const std::string&> breakingNews;
    
    void publishNews(const std::string& title, const std::string& content, const std::string& category)
    {
        NewsArticle article(title, content, category);
        std::cout << "Publishing: " << title << std::endl;
        newsPublished.notify(this, article);
    }
    
    void publishBreakingNews(const std::string& news)
    {
        std::cout << "BREAKING NEWS: " << news << std::endl;
        breakingNews.notify(this, news);
    }
};

// Подписчики на новости
class EmailSubscriber
{
public:
    EmailSubscriber(const std::string& name) 
        : _name(name)
    {

    }
    
    void onNewsReceived(const void* pSender, const NewsArticle& article)
    {
        std::cout << "[EMAIL to " << _name << "] New article: " 
                  << article.getTitle() << " (Category: " << article.getCategory() << ")\n";
    }
    
    void onBreakingNews(const void* pSender, const std::string& news)
    {
        std::cout << "[EMAIL to " << _name << "] URGENT: " << news << "\n";
    }
    
private:
    std::string _name;
};

class SMSSubscriber
{
public:
    void onBreakingNews(const void* pSender, const std::string& news)
    {
        // SMS имеют ограниченную длину
        std::string shortNews = news.substr(0, 30);
        if (news.length() > 30) shortNews += "...";
        std::cout << "[SMS] Breaking: " << shortNews << std::endl;
    }
};

int main()
{
    NewsPublisher publisher;
    EmailSubscriber alice("alice@example.com");
    EmailSubscriber bob("bob@example.com");
    SMSSubscriber smsService;

    publisher.newsPublished += Poco::Delegate<EmailSubscriber, const NewsArticle&>(
        &alice, &EmailSubscriber::onNewsReceived);
        
    publisher.newsPublished += Poco::Delegate<EmailSubscriber, const NewsArticle&>(
        &bob, &EmailSubscriber::onNewsReceived);
        
    publisher.breakingNews += Poco::Delegate<EmailSubscriber, const std::string&>(
        &alice, &EmailSubscriber::onBreakingNews);
        
    publisher.breakingNews += Poco::Delegate<SMSSubscriber, const std::string&>(
        &smsService, &SMSSubscriber::onBreakingNews);
    
    // Публикация новостей
    std::cout << "=== Regular News ===" << std::endl;
    publisher.publishNews("C++20 Released", "New features include...", "Technology");
    publisher.publishNews("Weather Forecast", "Sunny week ahead...", "Weather");
    
    std::cout << "\n=== Breaking News ===" << std::endl;
    publisher.publishBreakingNews("Major breakthrough in quantum computing announced!");

    // Боб отписывается от breaking news
    std::cout << "\n=== Alice unsubscribes from breaking news ===" << std::endl;
    publisher.breakingNews -= Poco::Delegate<EmailSubscriber, const std::string&>(
        &alice, &EmailSubscriber::onBreakingNews);
        
    publisher.publishBreakingNews("Stock market reaches all-time high!");
    
    return 0;
}

