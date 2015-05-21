/*************************************<+>*************************************
 *                                  MUSE
 *            Copyright  2015 Compro Simulation Systems, Inc.
 *
 *                            ALL RIGHTS RESERVED.
 *        USE, DISCLOSURE, OR REPRODUCTION WITHOUT WRITTEN PERMISSION
 *               OF COMPRO COMPUTER SERVICES, INC. IS PROHIBITED.
 *
 *  File     - globalidgenerator.h
 *
 *  Synopsis -
 *
 *  Author   - Trajce Nikolov
 *
 *  begin    - Wed May 6 2015
 *
 *  email    - trajce.nikolov.nick@gmail.com
 *
 ****************************************************************************/

#ifndef GLOBALIDGENERATOR_H
#define GLOBALIDGENERATOR_H

#include <IgCore/export.h>

#include <string>
#include <map>
#include <vector>

namespace igcore
{

/*! The inherits of \ref igcore::ImageGenerator are exepcted to implement ID
 *  based scene management of \ref igcore::ImageGenerator::Entity, Lights .. etc.
 *  There are situation when one will need an automated ID generations, for example
 *  when reading a model definition from a file containing ID based constructs like
 *  Lights. As a reference see the \ref ModelCompositionPlugin and \ref LightingPlugin
 *  implementations/
 * \brief Handy singleton class for ID numbers management
 * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
 * \copyright (c)Compro Computer Services, Inc.
 * \date      Sun Jan 11 2015
 */
class IGCORE_EXPORT GlobalIdGenerator
{
public:
    /*!
     * \brief The singleton
     * \return The singleton
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    static GlobalIdGenerator*  instance();

    /*!
     * \brief Inits a ID group by a name, base ID and the size of this group
     * \param The name of this ID group. You can have multiple groups identified by a name
     * \param The base ID. All consequent IDs are from this one and on
     * \param The size of the vector. If 10, then only 10 different IDs will be available
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    void initIdGroup(const std::string& group, unsigned int base, unsigned int size = 0);

    /*!
     * \brief Gets the next ID from a ID group
     * \param The name of the group ID
     * \param The next ID available
     * \return true on success, false if no more available IDs
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    bool getNextId(const std::string& group, unsigned int& id);

    /*! Sets list of IDs to be reused. Let explain it through an example. There is a
     * plugin available, see \ref RunwayLights that creates real Lights for a runway model
     * Once this is paged out in a paged visual database, the IDs of these lights can be
     * reused for another runway sitting on another tile
     * \brief Sets list of IDs to be reused
     * \param The ID group name
     * \param List of available IDs to be reused
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    void setAvailableIds(const std::string& group, const std::vector<unsigned int>& ids);

protected:
    GlobalIdGenerator ();
    ~GlobalIdGenerator();

    /*!
     * \brief The IdGroup struct. Internal for ID management
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    struct IdGroup
    {
        typedef std::vector<unsigned int>   IDs;

        unsigned int _base;
        IDs          _ids;

        IdGroup() : _base(0) {}
    };

    typedef std::map< std::string, IdGroup >    IdGroupMap;

    /*! \brief name based std::map of \ref IdGroup */
    IdGroupMap  _groups;
};

} // namespace

#endif // GLOBALIDGENERATOR_H

