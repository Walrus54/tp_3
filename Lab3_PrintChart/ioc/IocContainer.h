#ifndef IOCCONTAINER_H
#define IOCCONTAINER_H

#include <functional>
#include <map>
#include <memory>
#include <utility>

/**
 * @brief Канонический IoC-контейнер (вариант с лекций).
 *
 * Регистрирует фабрики по идентификатору типа и автоматически внедряет
 * зависимости конструктора: RegisterFactory<I, Impl, Dep1, Dep2>() создаёт Impl,
 * разрешая Dep1/Dep2 через сам контейнер.
 *
 * @note Регистрация не потокобезопасна. Сценарий — собрать весь граф в
 * композиционном корне (RegisterXxx), затем только разрешать (GetObject).
 */
class IocContainer
{
    inline static int s_nextTypeId = 1;

    // Уникальный устойчивый идентификатор для типа T.
    template <typename T>
    static int typeId()
    {
        static int id = s_nextTypeId++;
        return id;
    }

public:
    // Нетипизированный корень фабрики — узел в таблице factories_.
    class FactoryRoot
    {
    public:
        virtual ~FactoryRoot() = default;
    };

    template <typename T>
    class Factory : public FactoryRoot
    {
        std::function<std::shared_ptr<T>()> m_functor;

    public:
        explicit Factory(std::function<std::shared_ptr<T>()> functor)
            : m_functor(std::move(functor)) {}
        std::shared_ptr<T> create() { return m_functor(); }
    };

    /// @brief Разрешить объект интерфейса T (nullptr, если не зарегистрирован).
    template <typename T>
    std::shared_ptr<T> GetObject() const
    {
        auto it = m_factories.find(typeId<T>());
        if (it == m_factories.end())
            return nullptr;
        return std::static_pointer_cast<Factory<T>>(it->second)->create();
    }

    /// @brief Зарегистрировать функтор-сборщик с зависимостями TS... (внедрение зависимости).
    template <typename Interface, typename... TS>
    void RegisterFunctor(std::function<std::shared_ptr<Interface>(std::shared_ptr<TS>...)> functor)
    {
        m_factories[typeId<Interface>()] = std::make_shared<Factory<Interface>>(
            [this, functor] { return functor(GetObject<TS>()...); });
    }

    /// @brief Зарегистрировать готовый экземпляр (синглтон).
    template <typename Interface>
    void RegisterInstance(std::shared_ptr<Interface> instance)
    {
        m_factories[typeId<Interface>()] =
            std::make_shared<Factory<Interface>>([instance] { return instance; });
    }

    /**
     * @brief Зарегистрировать фабрику: новый экземпляр Concrete на каждый запрос.
     *
     * Зависимости конструктора TArgs... разрешаются контейнером автоматически.
     */
    template <typename Interface, typename Concrete, typename... TArgs>
    void RegisterFactory()
    {
        // Оборачиваем лямбду в явный std::function: вывод шаблона из лямбды
        // в std::function не работает, поэтому строим тип явно.
        RegisterFunctor(std::function<std::shared_ptr<Interface>(std::shared_ptr<TArgs>...)>(
            [](std::shared_ptr<TArgs>... args) -> std::shared_ptr<Interface> {
                return std::make_shared<Concrete>(std::move(args)...);
            }));
    }

    /// @brief Зарегистрировать синглтон Concrete (создаётся один раз, зависимости — из контейнера).
    template <typename Interface, typename Concrete, typename... TArgs>
    void RegisterSingleton()
    {
        RegisterInstance<Interface>(std::make_shared<Concrete>(GetObject<TArgs>()...));
    }

private:
    std::map<int, std::shared_ptr<FactoryRoot>> m_factories;
};

#endif // IOCCONTAINER_H
