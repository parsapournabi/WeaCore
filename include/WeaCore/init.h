#ifndef INIT_H
#define INIT_H

#include <QObject>

/*!
 * \brief WEACORE_NAMESPACE_START: module namespace start
 */
#define WEACORE_NAMESPACE_START namespace MODULE_NAME {

/*!
 * \brief WEACORE_NAMESPACE_END : module namespace end
 */
#define WEACORE_NAMESPACE_END };

/*!
 * \brief USE_NAMESPACE_WEACORE : module namespace using macro.
 */
#define USE_NAMESPACE_WEACORE using namespace MODULE_NAME;

#endif // INIT_H
