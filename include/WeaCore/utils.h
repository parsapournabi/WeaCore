#ifndef UTILS_H
#define UTILS_H

#include <QObject>

/** Properties declaration macros ( Only can use on Header ) **/

/*!
 *   @brief W_PROP_DECL: Wea Property Header Declaration.
 *   @param type: dataType.
 *   @param name: property getter name.
 *   @param cname: property capitalize name (setter method).
 *   @param value: default value.
 */
#define W_PROP_DECL(type, name, cname, value) \
    private: \
    Q_PROPERTY(type name READ name WRITE set##cname NOTIFY name##Changed) \
    public: \
        type name() const; \
        void set##cname(type); \
    signals: \
        void name##Changed(); \
    private: \
        type m_##name = value;


/*!
 *  @brief W_PROP_DECL_D: Wea Property Header Declaration Private (PIMPL).
 *  @param type: dataType.
 *  @param name: property getter name.
 *  @param cname: property capitalize name (setter method).
 */
#define W_PROP_DECL_D(type, name, cname) \
    private: \
    Q_PROPERTY(type name READ name WRITE set##cname NOTIFY name##Changed) \
    public: \
        type name() const; \
        void set##cname(type); \
    signals: \
        void name##Changed();


/*!
 *  @brief W_PROP_CPPDEF: Wea Property Cpp Definition.
 *  @param cls: Target Class Name.
 *  @param type: dataType.
 *  @param name: property getter name.
 *  @param cname: property capitalize name (setter method).
 */
#define W_PROP_CPPDEF(cls, type, name, cname) \
    type cls::name() const { return m_##name; } \
    void cls::set##cname(type p) { if (m_##name == p) return; m_##name = p; emit name##Changed(); }


/*!
 *  @brief W_PROP_CPPDEF_D: Wea Property Cpp Definition Private (PIMPL).
 *  @param cls: Target Class Name.
 *  @param type: dataType.
 *  @param name: property getter name.
 *  @param cname: property capitalize name (setter method).
 */
#define W_PROP_CPPDEF_D(cls, type, name, cname) \
    type cls::name() const { Q_D(cls); return d->m_##name; } \
    void cls::set##cname(type p) { Q_D(cls); if (d->m_##name == p) return; d->m_##name = p; emit name##Changed(); }


/** Properties definitions macros ( Only can use on Header ) **/

/*!
 *  @brief W_PROP_HDEF: Wea Property Header Definition.
 *  @param type: dataType.
 *  @param name: property getter name.
 *  @param cname: property capitalize name (setter method).
 *  @param value: default value.
 */
#define W_PROP_HDEF(type, name, cname, value) \
    private: \
    Q_PROPERTY(type name READ name WRITE set##cname NOTIFY name##Changed) \
    public: \
        type name() const { return m_##name; } \
        void set##cname(type p) { if (m_##name == p) return; m_##name = p; emit name##Changed();} \
        Q_SIGNAL void name##Changed(); \
    private: \
        type m_##name = value;


/*!
 *  @brief W_PROP_HDEF_D: Wea Property Header Definition Private (PIMPL).
 *  @param cls: Target Class Name.
 *  @param type: dataType.
 *  @param name: property getter name.
 *  @param cname: property capitalize name (setter method).
 */
#define W_PROP_HDEF_D(cls, type, name, cname) \
    private: \
    Q_PROPERTY(type name READ name WRITE set##cname NOTIFY name##Changed) \
    public: \
        inline type name() const { Q_D(cls); return d->m_##name; } \
        inline void set##cname(type p) { Q_D(cls); if (d->m_##name == p) return; d->m_##name = p; emit name##Changed(); } \
    signals: \
        void name##Changed();


#endif // UTILS_H
