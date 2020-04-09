/*!
 * \file Cpp11EventSystem.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 02/05/2016 11:21:19
 *
 *
 */
#pragma once

#include "BaseType.h"

#include <functional>
#include <map>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

NS_BEGIN

class Event
{
public:
    Event(){};
    virtual ~Event(){};
};

class BaseEventCallback
{
public:
    BaseEventCallback(){};
    virtual ~BaseEventCallback(){};

    virtual void invoke(Event& event) = 0;
};

template <typename T> class EventCallback : public BaseEventCallback
{
public:
    EventCallback(const std::function<void(T& evt)>& callback)
        : m_callback(callback){};
    virtual ~EventCallback(){};

    virtual void invoke(Event& evt) override { m_callback(static_cast<T&>(evt)); }

private:
    std::function<void(T& evt)> m_callback;
};

class EventHandler
{
public:
    class EventTargetHost
    {
    public:
        virtual ~EventTargetHost()
        {
            for (auto sender : m_senderSet)
            {
                for (auto target : m_targetSet)
                {
                    sender->unregisterEvent(target);
                }
                sender->m_eventHosts.erase(this);
            }
        }

        void addHostInfo(EventHandler* sender, void* target)
        {
            m_senderSet.insert(sender);
            m_targetSet.insert(target);
        }

        void removeSender(EventHandler* sender) { m_senderSet.erase(sender); }

    private:
        std::unordered_set<EventHandler*> m_senderSet;
        std::unordered_set<void*> m_targetSet;
    };

    class EventInfo
    {
    public:
        void* m_target{};
        BaseEventCallback* m_callback{};
    };

    typedef std::multimap<std::type_index, EventInfo> EventInfoMap;

    typedef std::unordered_set<EventTargetHost*> EventHostSet;

public:
    EventHandler(){};

    virtual ~EventHandler()
    {
        for (auto host : m_eventHosts)
        {
            host->removeSender(this);
        }
        m_eventHosts.clear();

        for (auto it : m_eventInfoMap)
        {
            delete it.second.m_callback;
        }
        m_eventInfoMap.clear();
    }

    template <typename T> bool registerEvent(EventTargetHost* host, void* target, const std::function<void(T& evt)>& callback)
    {
        EventInfo eventInfo;
        eventInfo.m_target = target;
        eventInfo.m_callback = new EventCallback<T>(callback);

        m_eventInfoMap.insert(std::make_pair(std::type_index(typeid(T)), eventInfo));

        host->addHostInfo(this, target);

        m_eventHosts.insert(host);
        return true;
    }

    void unregisterEvent(void* target)
    {
        for (EventInfoMap::iterator it = m_eventInfoMap.begin(); it != m_eventInfoMap.end();)
        {
            const EventInfo& eventInfo = it->second;
            if (eventInfo.m_target == target)
            {
                delete eventInfo.m_callback;
                auto itPrev = it;
                ++it;
                m_eventInfoMap.erase(itPrev);
            }
            else
            {
                ++it;
            }
        }
    }

    template <typename T> int postEvent(T& evt)
    {
        auto range = m_eventInfoMap.equal_range(typeid(T));
        if (range.first == range.second) return 0;

        // clone event info
        std::vector<EventInfo> eventInfoList;
        for (auto it = range.first; it != range.second; ++it)
        {
            EventInfo& handler = it->second;
            eventInfoList.push_back(handler);
        }

        // post event
        for (auto eventInfo : eventInfoList)
        {
            eventInfo.m_callback->invoke(evt);
        }

        return eventInfoList.size();
    }

protected:
    EventInfoMap m_eventInfoMap;
    EventHostSet m_eventHosts;
};

NS_END

#define DECLARE_EVENT_HOST()                                                                                                                                   \
private:                                                                                                                                                       \
    NS::EventHandler::EventTargetHost __event_host__

#define REGISTER_EVENT(sender, eventType, callback) (sender)->registerEvent<eventType>(&__event_host__, this, std::bind(callback, this, std::placeholders::_1))
