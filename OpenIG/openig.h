//#******************************************************************************
//#*
//#*      Copyright (C) 2015  Compro Computer Services
//#*      http://openig.compro.net
//#*
//#*      Source available at: https://github.com/CCSI-CSSI/MuseOpenIG
//#*
//#*      This software is released under the LGPL.
//#*
//#*   This software is free software; you can redistribute it and/or modify
//#*   it under the terms of the GNU Lesser General Public License as published
//#*   by the Free Software Foundation; either version 2.1 of the License, or
//#*   (at your option) any later version.
//#*
//#*   This software is distributed in the hope that it will be useful,
//#*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
//#*   the GNU Lesser General Public License for more details.
//#*
//#*   You should have received a copy of the GNU Lesser General Public License
//#*   along with this library; if not, write to the Free Software
//#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//#*
//#*****************************************************************************
#ifndef OPENIG_H
#define OPENIG_H

#include <OpenIG/export.h>
#include <OpenIG/keypad.h>

#include <IgCore/imagegenerator.h>
#include <IgCore/attributes.h>

#include <IgPluginCore/pluginhost.h>
#include <IgPluginCore/plugincontext.h>

#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

using namespace igcore;

namespace openig
{

/*! Implementation of \ref igcore::ImageGenerator
 * \brief The OpenIG class
 * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
 * \copyright (c)Compro Computer Services, Inc.
 * \date      Fri Jan 16 2015
 */
class OPENIG_EXPORT OpenIG :
        public igcore::ImageGenerator,
        public igplugincore::PluginHost,
        public osg::Referenced
{
public:
    /*!
     * \brief Constructor
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    OpenIG();
    /*!
     * \brief Destructor
     */
    virtual ~OpenIG();

    /*!
     * \brief Load a script with commands on startup
     * \param fileName The sctipt file name
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    virtual void loadScript(const std::string& fileName);

    /*!
     * \brief Gets the version
     * \return The version
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual std::string version();

    /*! Should be called immediatelly after making an instance. Here the setup of the
     *  scene and other things happen. By default \ref openig::OpenIG scene is
     *  osgShadow::ShadowedScene with LightSpacePerspective shadow map technique.
     *  So the user is expected to add nodes directly to \ref getScene, or if they
     *  want to handle it their own way, there is still osgView::View(x)->getSceneData()
     *  \brief Performs init of the \ref ImageGenerator.
     *  \param viewer       Your Viewer. It is up to you how you set up
     *                      expected is to have at least one View.
     *  \param xmlFileName  The file name of the xml configuration file.
     *                      Defaults to openig.xml, on Windows in bin/igdata
     *                      MacOS and Linux in /usr/local/bin/igdata
     *  \return             Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void init(osgViewer::CompositeViewer* viewer, const std::string& xmlFileName = "openig.xml");

    /*! Call it before destruction
     *  \brief Performs cleanup.
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void cleanup();

    /*! It calls viewer->frame() and performs internal calling of plugins hooks
     *  like, update, preFrame, postFrame. This method mimics the viewer->frame()
     *  call, only inserts hooks for \ref igplugincore::Plugin methods, in this order,
     *  as is implemented in \ref openig::OpenIG::frame :
     *      \code{.cpp}
     *       // pseudocode
     *       // call these first
     *       _viewer->advance();
     *       _viewer->eventTraversal();
     *       _viewer->updateTraversal();
     *
     *       // here we call the igplugincore::Plugin::update on all plugins
     *       // via igplugincore::PluginOperation
     *       igplugincore::Plugin::update
     *
     *       // here ImageGenerator::preRender is called
     *       igcore::ImageGenerator::preRender
     *
     *       // here we call the igplugincore::Plugin::preFrame on all plugins
     *       // via igplugincore::PluginOperation
     *       igplugincore::Plugin::preFrame
     *
     *       // call the rendering
     *       _viewer->renderingTraversals();
     *
     *       // here we call the igplugincore::Plugin::postFrame on all plugins
     *       // via igplugincore::PluginOperation
     *       igplugincore::Plugin::postFrame
     *
     *       // here ImageGenerator::postRender is called
     *       igcore::ImageGenerator::postRender
     *      \endcode
     *  \brief The frame, should be called in a loop.
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void frame();

	/*! Sets the Read node callback. Some might want to change how the file
	*	is read, like osgEarth for example. You have an option
	*	to read files differently then with osgDB::readNodeFile(...)
	*  \brief Sets the read node callback
	*  \param cb The callback
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Mon Jun 16 2015
	*/
	virtual void setReadNodeImplementationCallback(ReadNodeImplementationCallback* cb);

	/*! Gets the Read node callback. See \ref setReadNodeImplementationCallback
	*  \brief Gets the read node callback
	*  \return cb The callback
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Mon Jun 16 2015
	*/
	virtual ReadNodeImplementationCallback* getReadNodeImplementationCallback();


    /*! \ref OpenIG is using IDs for everything in the
     *  scene management. The IDs are mainly up to the user to maintain. However
     *  some plugins generates these IDs automatically, for example the \ref
     *  ModelCompositionPlugin or the \ref LightingPlugin. For the automated
     *  ID generations there is a class \ref igcore::GlobalIdGenerator. 
     *  \brief Adds \ref Entity in the scene.
     *  \param id       The id of the \ref Entity. You should use this Entity id to refer
     *                  to this model in the scene
     *  \param fileName The file name of the model
     *  \param mx       Initial position as osg::Matrixd
     *  \param options  Optional. The option string can be processed by some plugins.
     *                  Good example can be the VDBOffset which shifts the database
     *                  by a given offset defined in the osgDB::Options string.
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void addEntity(unsigned int id, const std::string& fileName, const osg::Matrixd& mx, const osgDB::Options* options = 0);

	/*! Adds entity in the scene from osg::Node instead of loading it from a file. See \ref addEntity
	*  \brief Adds entity in the scene from osg::Node instead of loading it from a file.
	*  \param id       The id of the \ref Entity. You should use this Entity id to refer
	*                  to this model in the scene
	*  \param node	   The node to become an \ref Entity
	*  \param mx       Initial position as osg::Matrixd
	*  \param options  Optional. The option string can be processed by some plugins.
	*                  Good example can be the VDBOffset which shifts the database
	*                  by a given offset defined in the osgDB::Options string.
	*  \return         Nothing
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Tue Jun 16 2015
	*/
	virtual void addEntity(unsigned int id, const osg::Node* node, const osg::Matrixd& mx, const osgDB::Options* options = 0);

    /*! Removes an entity from the scene. See \ref addEntity
     *  immediatelly, still in the same frame though and it processed in ViewerOperation.
     *  When one inherits, it should maintain std::map of Entities available through \ref
     *  getEntitiesToRemoveMap
     *  \brief Removes \ref Entity from the scene.
     *  \param id       The id of the \ref Entity. This is the id you have used with \ref addEntity
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void removeEntity(unsigned int id);

    /*! Updates \ref Entity with new position and orientation by using a osg::Matrixd.
     *  \brief Updates \ref Entity
     *  \param id       The id of the \ref Entity. This is the id you have used with \ref addEntity
     *  \param mx       The new position and orientation of the \ref Entity. It is handy to use
     *                  \ref igcore::Math methods to contruct this Matrix, as toMatrix(...)
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void updateEntity(unsigned int id, const osg::Matrixd& mx);

    /*! Show/Hide Entity
     *  \brief Show/Hide Entity
     *  \param id       The id of the \ref Entity. This is the id you have used with \ref addEntity
     *  \param show     If true, the \ref Entity will be present in the scene, if false it will not
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void showEntity(unsigned int id, bool show);

    /*! Attach one \ref Entity to another. Then it makes it Sub-Entity. The offset is defined
     *  with the osg::Matrixd used in update \ref Entity. Then this position and orientation
     *  become local to the parent \ref Entity
     *  \brief Attach one \ref Entity to another.
     *  \param id           The id of the \ref Entity. This is the id you have used with \ref addEntity
     *  \param toEntityId   The id of the parent \ref Entity
     *  \return             Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void bindToEntity(unsigned int id, unsigned int toEntityId);

    /*! Dettach an \ref Entity from its parent \ref Entity.  Internaly the position and orientation is
     *  computed from local to world space on detach so the \ref Entity stays on
     *  its current position and orientation
     *  \brief Detach an \ref Entity from its parent \ref Entity.
     *  \param id           The id of the attached \ref Entity. This is the id you have used with \ref addEntity
     *  \return             Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void unbindFromEntity(unsigned int id);

    /*! You might want to replace the model with another and preserve the id. See \ref addEntity
     *  \param id       The id of the attached \ref Entity. This is the id you have used with \ref addEntity
     *  \param fileName The file name of the model. Can be the same or new
     *  \param options  Optional. Some plugins can use the option string from the osgDB::Option object
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void reloadEntity(unsigned int id, const std::string& fileName, const osgDB::Options* options = 0);

    /*! Sets a name for an \ref Entity. Can be handy for lookup
     *  \brief Sets a name for an \ref Entity
     *  \param id       The id of the attached \ref Entity. This is the id you have used with \ref addEntity
     *  \param name     The \ref Entity name
     *  \return         Nothing
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void setEntityName(unsigned int id, const std::string& name);

    /*! Gets the name for an \ref Entity. Can be handy for lookup
     *  \brief Geets a name for an \ref Entity
     *  \param id       The id of the attached \ref Entity. This is the id you have used with \ref addEntity
     *  \return         The \ref Entity name
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Mon May 18 2015
     */
    virtual std::string getEntityName(unsigned int id);

    /*! Plays animation on an \ref Entity. At present it uses the internal simple animation handling
     *  - see \ref igcore::Animations. The future versions will have FBX support as well. The
     *  present animations are defined in a model XML file and there are plugins that can support
     *  the simple animation management implemented in this core. As a reference can be the
     *  \ref ModelComposition plugin that is reading this XML defined animations and make all the
     *  necessary steps to crate the build-in animations from XML definitions. The reason to have it
     *  this way, is the need of controlling the animation via run-time value from code. The pre-baked
     *  animations are somewhat complicated to control based on some run-time value, so we were to
     *  implement our own management that is allowing us to control the players in the animations as
     *  we control \ref Entity es, by a run-tome value, with a Matrix.
     * \brief Plays animation on an \ref Entity
     * \param entityId          The id of the \ref Entity to play the animation
     * \param animationName     The name of the animation, as defined in the model XML or for future
     *                          versions the hame of the FBX animation
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void playAnimation(unsigned int entityId, const std::string& animationName);

    /*! Plays multiple animations on an \ref Entity at once. For more info how the animations are
     *  supported pleas see \ref igcore::ImageGenerator::playAnimation
     * \brief Plays multiple animations on an \ref Entity at once
     * \param entityId          The id of the \ref Entity to play the animations
     * \param animations        The name of the animations, as defined in the model XML or for future
     *                          versions the hame of the FBX animations
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void playAnimation(unsigned int entityId, const StringUtils::StringList& animations);

    /*! Plays animation on an \ref Entity with an animation callback. For more info how the animations are
     *  supported pleas see \ref igcore::ImageGenerator::playAnimation. The addition to basic animation
     *  playback is the list of \ref igcore::AnimationSequencePlaybackCallback from \file animation.h which
     *  gives us ability to control animation sequences in run-time by a run-time value. For example we can
     *  give landing gear failure: If we have animation animating a landing gear and it is defined to start and\
     *  run to completition, we can stop the animation sequence by a run-time value, for example the  landing
     *  gear at 60 degrees from its start position
     * \brief Plays animation on an \ref Entity with a callbacks attached
     * \param entityId          The id of the \ref Entity to play the animations
     * \param animations        The name of the animations, as defined in the model XML or for future
     *                          versions the hame of the FBX animations
     * \param cbs               Referenced std::vector of \ref igcore::AnimationSequencePlaybackCallback
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void playAnimation(unsigned int entityId, const std::string& animationName, RefAnimationSequenceCallbacks* cbs );

    /*! Stops the playback of animation by name and \ref Entity ID. See \ref playAnimation
     * \brief Stops the playback of animation
     * \param entityId          The ID of the entity
     * \param animationName     The name of the animation
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      MOn May 18 2015
     */
    virtual void stopAnimation(unsigned int entityId, const std::string& animationName);

    /*! Stops the playback of multiple animations by their names and \ref Entity ID. See \ref playAnimation
     * \brief Stops the playback of animation
     * \param entityId          The ID of the entity
     * \param animations        The names of the animations
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      MOn May 18 2015
     */
    virtual void stopAnimation(unsigned int entityId, const StringUtils::StringList& animations);

    /*! Reset the playback of animation by name and \ref Entity ID. It placed the players at their
     *  initial position before the playback. See \ref playAnimation
     * \brief Reset the playback of animation
     * \param entityId          The ID of the entity
     * \param animationName     The name of the animation
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      MOn May 18 2015
     */
    virtual void resetAnimation(unsigned int entityId, const std::string& animationName);

    /*! Reset the playback of multiple animations by their names and \ref Entity ID. It places the players at
     *  their initial position before the playback. See \ref playAnimation
     * \brief Reset the playback of multiple animations at once
     * \param entityId          The ID of the entity
     * \param animations        The names of the animations
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      MOn May 18 2015
     */
    virtual void resetAnimation(unsigned int entityId, const StringUtils::StringList& animations);


	// Effects
	/*! Adds effect to the scene. This oonly manages internal structures, it
	*	is up to plugins to provide implementation
	*  \brief Adds effect to the scene
	*  \param id	Unique effect to the scene
	*  \param name	Name of the effect
	*  \param mx	The initial position/orientation of the effect
	*  \param attributes String based attributes for the pugins provoding the implementation, in form of token=attr;token=attr ...
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void addEffect(unsigned int id, const std::string& name, const osg::Matrixd& mx, const std::string& attributes);

	/*! Removes effect from the scene
	*  \brief Removes effect from the scene
	*  \param id	Unique effect to the scene
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void removeEffect(unsigned int id);

	/*! Binds effect to an \ref Entity
	*  \brief Binds effect to an \ref Entity
	*  \param id		Unique effect to the scene
	*  \param entityID	The ID of the \ref Entity
	*  \param mx		The offset position/orientation of the effect
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void bindEffect(unsigned int id, unsigned int entityID, const osg::Matrixd& mx);

	/*! Unbinds effect to an \ref Entity
	*  \brief Unbinds effect to an \ref Entity
	*  \param id		Unique effect to the scene
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void unbindEffect(unsigned int id);

	/*! Update effect with new position/orientation
	*  \brief Update effect with new position/orientation
	*  \param id		Unique effect to the scene
	*  \param mx		The new position/orientation
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void updateEffect(unsigned int id, const osg::Matrixd& mx);

	/*! The \ref ImageGenerator, \ref OpenIG only manages structures for the
	*	effects. It is up to plugins to provide their implementation. This is
	*	the callback that provides the effect implementation
	*  \brief Sets the effect implementation callback
	*  \param cb		The callback
	*  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	*  \copyright (c)Compro Computer Services, Inc.
	*  \date      Sun Jun 14 2015
	*/
	virtual void setEffectImplementationCallback(GenericImplementationCallback* cb);


    /*! Sets the position of the camera using Matrix. If the camera is bind to an
     *  \ref Entity, then this Matrix is the local offset. One can check if the camera
     *  is bound by \ref isCameraBoundToEntity
     *  \brief Set the position of the camera
     *  \param mx            The inital position of the camera. World coordinates
     *  \param viewMatrix    Set true if the provided Matrix is View Matrix
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void setCameraPosition(const osg::Matrixd& mx, bool viewMatrix = false);

    /*! Binds the camera to an \ref Entity. Then as the \ref Entity is moving, the Camera
     *  is movving along with. The given Matrix is the local offset
     *  \brief Binds camera to an \ref Entity
     *  \param id            The id of the \ref Entity, the one you have used with \ref addEntity
     *  \param mx            The local offset wrt. to the \ref Entity
     *  \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     *  \copyright (c)Compro Computer Services, Inc.
     *  \date      Sun Jan 11 2015
     */
    virtual void bindCameraToEntity(unsigned int id, const osg::Matrixd& mx);

    /*! Updates the camera if it is bound to an \ref Entity with new position and
     *  orientation through Matrix. Might be handy to use \ref igcore::Math methods
     *  like \ref igcore::Math::toMatrix to construct the Matrix
     * \brief Updates the camera.
     * \param mx    The local offset wrt. to the \ref Entity
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void bindCameraUpdate(const osg::Matrixd& mx);

    /*! Unbinds the Camera from an \ref Entity if it is already bound
     * \brief Unbinds the Camera from an \ref Entity
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void unbindCameraFromEntity();

    /*! Checks if the Camera is bound to an \ref Entity
     * \brief Checks if the Camera is bound to an \ref Entity
     * \return      true if it is bounds, false otherwise
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual bool isCameraBoundToEntity();

    /*! Sets for fixed up Cametra orientation or not. When is set with false,
     *  and is bound to an \ref Entity it follows the \ref Entity orientation,
     *  as an example can be the Camera attached to a plane model to have pilot
     *  view. When set with true, then the up axis is fixed and the Camera is not
     *  following the \ref Entity orientation, like a bird eye.
     * \brief Sets for fixed up Cametra orientation or not
     * \param       fixedUp true to fix the up axis or false to follow the \ref Entity orientation
	 * \param       freezeOrientation true to freeze orientation
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
	virtual void bindCameraSetFixedUp(bool fixedUp, bool freezeOrientation = false);

    /*! Sets fog in the scene. The \ref openig::OpenIG is adding a Fog attribute in the scene
     *  and it is up to plugins to implement it's appearance. Example is the \ref LightingPlugin which
     *  computes the fog in a shader. Also, internaly \ref openig::OpenIG is creating \ref igcore::FogAttributes
     *  via \ref igplugincore::PluginContext::Attribute and passing it to all the plugins to deal with.
     * \brief Sets fog in the scene
     * \param visibility    Visibility in meters
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setFog(double visibility);

    /*! Sets the time of day. The core is not implementing this feature instead is creating
     *  \ref igcore::TimeOfDayAttributes via \ref igplugincore::PluginContext::Attribute and
     *  it is passing it to all the plugins to deal with.
     * \brief Sets the time of day.
     * \param hour
     * \param minutes
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setTimeOfDay(unsigned int hour, unsigned int minutes);

    /*! Sets rain in the scene. The \ref openIG::OpenIG is not implementing but instead it creates
     *  \ref igcore::RainSnowAttributes via \ref igplugincore::PluginContext::Attribute and it is
     *  passing it to all the plugins to deal with
     * \brief Sets rain in the scene
     * \param factor    Can be anything, however some plugins that implements atmospheric effects
     *                  like the \ref SilverLiningPlugin is expecting this in the range of 0.0-1.0
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setRain(double factor);

    /*! Sets snow in the scene. The \ref openIG::OpenIG is not implementing but instead it creates
     *  \ref igcore::RainSnowAttributes as \ref igplugincore::PluginContext::Attribute and it is
     *  passing it to all the plugins to deal with
     * \brief Sets snow in the scene
     * \param factor    Can be anything, however some plugins that implements atmospheric effects
     *                  like the \ref SilverLiningPlugin is expecting this in the range of 0.0-1.0
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setSnow(double factor);

    /*! Sets wind in the scene. The \ref openIG::OpenIG is not implementing but instead it creates
     *  \ref igcore::RainSnowAttributes via \ref igplugincore::PluginContext::Attribute and it is
     *  passing it to all the plugins to deal with
     * \brief Sets wind in the scene
     * \param speed     Speed of the wind. Can be anything or plugin specific. Good to mention here
     *                  is that it might be expected by plugins to be in m/s/
     * \param direction The direction. Can be anything or plugin specific. The \ref SilverLiningPlugin
     *                  is expecting this to be in degrees from North.
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setWind(float speed, float direction = 0.f);

    /*! Adds cloud layer in the scene. The \ref openig::OpenIG is not implementing this feature instead
     *  it creates \ref igcore::CLoudLayerAttributes via \ref igplugincore::PluginContext::Attribute
     *  and it is passing it to all the plugins. As a reference
     *  that implements this feature can be the \ref SilverLiningPlugin. Also the cloud layers are expected
     *  to be ID based, the id provided here is the handle for any clouds operation you might use
     * \brief Adds cloud layer in the scene.
     * \param id        The id of the cloud layer
     * \param type      The type of the cloud layer. Can be anything, or plugin specific
     * \param altitude  The attitude of the cloud layer
     * \param thickness The thickness of the cloud layer
     * \param density   The density of the cloud layer. Can be anything although some plugins as the
     *                  \ref SilverLiningPlugin is expecting this to be in the range of 0.0-1.0
     * \param width     The width of the cloud layer
     * \param length    The length of the cloud layer
     * \param infinity  Does the cloud layer go on forever?
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void addCloudLayer(unsigned int id, int type, double altitude, double thickness=0, double density=0.3);

    /*! Removes a cloud layer from the scene. The \ref openig::OpenIG is not implementing this instead it uses
     *  \ref igplugincore::PluginContext::Attribute to pass commands to all the plugins that might deal with
     *  such atmosphere effects. See \ref addCloudLayer for more info
     * \brief Removes a cloud layer from the scene
     * \param id    The id of the clouds layer to be removed
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void removeCloudLayer(unsigned int id);

    /*! Updates a cloud layer in the scene with new values. The openig::OpenIG is not implementing this instead
     *  it uses \ref igplugincore::PluginContext::Attribute to pass commands to all the plugins that might deal with
     *  such atmosphere effects. See \ref addCloudLayer for more info
     * \brief Update a cloud layer in the scene
     * \param id            The id of the cloud layer
     * \param altitude      Altitude of the cloud layer
     * \param thickness     Thickness of the cloud layer
     * \param density       Density of the cloud layer. This can be anything but some plugins that
     *                      implements atmospheric effect is the \ref SilverLiningPlugin is is
     *                      expecting this to be in the range of 0.0-1.0
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void updateCloudLayer(unsigned int id, double altitude, double thickness, double density);

    /*!
     * \brief Removes all cloud layers from the scene. The \ref openig::OpenIG is not implementing this instead it uses
     *  \ref igplugincore::PluginContext::Attribute to pass commands to all the plugins that might deal with
     *  such atmosphere effects. See \ref addCloudLayer for more info
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void removeAllCloudlayers();

    /*! Adds light source in the scene. As all others scene players, like \ref Entity, Lights are
     *  ID based. The ID management is up to the user, or one can use the simple igcore::GlobalIdGenerator.
     *  Actually the igcore::GlobalIdGenerator is used by plugins to create lights for the scene
     *  that might be defined in XML. We will mention the \ref ModelCompositionPlugin and the \ref
     *  LightingPlugin which use automatic ID generation using the igcore::GlobalIdGenerator class.
     *  The initial position and orientation of the lightsource is provided by a Matrix. If the light
     *  is bound to the Camera or an \ref Entity this Matrix defines the local to Camera \ref Entity offset
     *  ID 0 is reserved for scene sun/moon light
     * \brief Adds light source in the scene
     * \param id    The id of the light
     * \param mx    The initial position and orientation of the light. You might want to use \ref igcore::Math
     *              to create this matrix
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void addLight(unsigned int id, const osg::Matrixd& mx);

    /*! Removes light from the scene. See \ref addLight for more info.
     * \brief Removes light from the scene
     * \param id    The ID of the light to be removed
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void removeLight(unsigned int id);

    /*! Updates the light position and orientation in the scene. If bound to the Camera or \ref Entity
     *  it is updating the local offset. You might want to use \ref igcore::Math to construct the needed
     *  Matrix for the update. See \ref addLight for more info.
     * \brief Updates a light in the scene
     * \param id    The ID of the light.
     * \param mx    New position and orientation as Matrix
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void updateLight(unsigned int id, const osg::Matrixd& mx);

    /*! Binds a light to an \ref Entity. If you need to understand the ID paradigm refer to \ref
     *  addEntity and \ref addLight.
     * \brief Binds a light to an \ref Entity
     * \param id            The ID of the light
     * \param entityId      The ID of the \ref Entity
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void bindLightToEntity(unsigned int id, unsigned int entityId);

    /*! Unbinds light from an \ref Entity, if bound. Please refer to \ref addLight to get more
     *  info about lights ID and their management
     * \brief Unbinds light from an \ref Entity, if bound
     * \param id    The ID of the light to be unbind
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void unbindLightFromEntity(unsigned int id);

    /*! Enables/disables light in the scene. Please refer to \ref addLight to get more
     *  info about lights ID and their management
     * \brief Enables/disables light in the scene
     * \param id        The ID of the light to enable/disable
     * \param enable    true for enable, false for disable
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void enableLight(unsigned int id, bool enable);

    /*! Binds light to a Camera. Once this is called, the light follows the Camera moves
     *  and it is attached to a local Camera offset defined by the Matrix. You might want
     *  to use the \ref igcore::Math class to contruct the Matrix. To get more info about
     *  light implementation, light IDs and their management please refer to \ref
     *  setLightImplementationCallback and \ref addLight
     * \brief Binds light to a Camera
     * \param id        The ID of the light.
     * \param offset    The offset wrt. the Camera
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void bindLightToCamera(unsigned int id, const osg::Matrixd& offset = osg::Matrixd::identity());

    /*! Unbinds the light from a Camera, if bound. See \ref bindLightToCamera
     * \brief Unbinds the light from a Camera
     * \param id    The ID of the light
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void unbindLightFromcamera(unsigned int id);

    /*! Test if a light is enabled. See \ref addLight for more infor about lights IDs and
     *  their management.
     * \brief Test if a light is enabled
     * \param id    The ID of the light
     * \return      true if the light is enabled, see \ref enableLight, false otherwise
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual bool isLightEnabled(unsigned int id);

	/*! Returns the LightAttributes map. The inheritants are expected to
	*	keep track when they are updates. Plugins might use these
	* \brief Returns the LightAttributes map
	* \return The light attributes map
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sat Jun 06 2015
	*/
	virtual LightAttributesMap& getLightAttributesMap();

	/*! Returns the LightAttributes for a given light. The inheritants are expected to
	*	keep track when they are updates. Plugins might use these
	* \brief Returns the LightAttributes based on the light ID
	* \param id The light ID
	* \return The light attributes of the Light
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sat Jun 06 2015
	*/
	virtual LightAttributes getLightAttributes(unsigned int id);

    /*! Updates light attributes via \ref igcore::LightAttributes . Once you set the new
     *  light attributes, be aware that you have to set the dirty mask there as well
     *  in order to have the update. Please see \ref setLightImplementationCallback for more
     *  info about lights and their management
     * \brief Update lights attributes, like colors etc..
     * \param id        The ID of the light you used with \ref addLight
     * \param attribs   The new attributes
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void updateLightAttributes(unsigned int id, const igcore::LightAttributes& attribs);

    /*! Override preRender method to be called from within a frame. See \ref frame for reference
     * \brief override preRender method to be called from within a frame
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void preRender();

    /*! Override postRender method to be called from within a frame. See \ref frame for reference
     * \brief override postRender method to be called from within a frame
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void postRender();

    /*!
     * \brief Sets to update any CameraMainpulator coming with the viewer on \ref openig::OpenIG
     * \param update if true updates it when it updates the Camera Position, false to ignore
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void setUpdateViewerCameraManipulator(bool update);

    /*!
     * \brief Represntation of LightEntity
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    typedef osg::ref_ptr<osg::MatrixTransform>                   LightEntity;
    typedef std::map<unsigned int, LightEntity >                 LightsMap;
    typedef std::map<unsigned int, LightEntity >::iterator       LightsMapIterator;
    typedef std::map<unsigned int, LightEntity >::const_iterator LightsMapConstIterator;

    /*! Returns the reference for the light implementation callback. See
     * \ref igcore::ImageGenerator::setLightImplementationCallback
     * \brief Returns the reference for the light implementation callback
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    virtual igcore::LightImplementationCallback* getLightImplementationCallback();

    /*! Sets the light implementation callback. \ref openig::OpenIG is not implementing lights. It only
     *  manages some structures and it is up to plugins to give some lighting implementation. At the
     *  time of writing this, there are two that implements lighting in shaders: \ref SimlpeLightingPlugin
     *  and \ref LightingPlugin
     * \brief Sets the light implementation callback
     * \param cb    The callback, expected to be implemented by a plugin
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual void setLightImplementationCallback( igcore::LightImplementationCallback* cb);

    /*! Returns the viewer used in \ref init . Handy when using the \ref igcore::ImageGenerator in
     *  \ref igplugincore::PluginContext given to plugins \ref igplugincore::Plugin to access the
     *  viewer from within plugins
     * \brief Returns the viewer.
     * \return  The viewer passed in \ref init
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual osgViewer::CompositeViewer* getViewer();

    /*! Returns the node representing the managed scene. In \ref openig::OpenIG this is the
     *  default osgShadow::ShadowedScene. Adding nodes to this scene will cause these nodes
     *  to be affected by plugins, as for example by the \ref LightingPlugin which will have\
     *  shaders applied to them.
     * \brief Get the managed scene
     * \return  The managed scene. See \ref init
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual osg::Node*                  getScene();

    /*! The ID based \ref Entity std::map. Inheritants are expected to maintain this map along
     *  with all the scene management methods, as \ref addEntuty etc ... \ref openig::OpenIG is
     *  doing so
     * \brief The ID based \ref Entity std::map
     * \return  The recent ID based \ref Entity std::map
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual EntityMap&                  getEntityMap();

    /*! The light that represents sun/moon in the scene. It is the reserved light with an ID of 0
     * \brief   The light that represents sun/moon in the scene
     * \return  The light that represents sun/moon in the scene
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual osg::LightSource*           getSunOrMoonLight();

    /*! The scene Fog attribute. See \ref setFog for more info
     * \brief The scene fog
     * \return The scene Fog attribute
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Sun Jan 11 2015
     */
    virtual osg::Fog*                   getFog();

    /*!
     * \brief Returns the internal igplugincore::PluginContext
     * \return The internal igplugincore::PluginContext
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    igplugincore::PluginContext&        getPluginContext();

    /*!
     * \brief Returns the ID based std::map  of \ref LightEntity
     * \return
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    LightsMap&                          getLightsMap();

	/*!
	* \brief Returns the list of files that use file cache
	* \return	The list of files that use file cache
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sat Jun 27 2015
	*/
	const igcore::StringUtils::StringList& getFilesToBeCached() const;

	/*!
	* \brief	Adds files to use the cache
	* \param	fileList	The list of files that will use file cache
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sat Jun 27 2015
	*/
	void addFilesToBeCached(const igcore::StringUtils::StringList& files);

	/*!
	* \brief	Returns true if the given file is using the file cache
	* \return	Returns true if the given file is using the file cache
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sat Jun 27 2015
	*/
	bool isFileCached(const std::string& fileName);

protected:

    /*! \brief  The current added in the scene \ref Entity es, ID based std::map */
    EntityMap                                       _entities;

    /*! \brief  Handle of the viewer you have passed in \ref init */
    osg::ref_ptr<osgViewer::CompositeViewer>        _viewer;
    /*! \brief  Handle of the sun/moon light source with reserved ID of 0*/
    osg::ref_ptr<osg::LightSource>                  _sunOrMoonLight;
    /*! \brief  Handle of Fog attribute */
    osg::ref_ptr<osg::Fog>                          _fog;
    /*! \brief  The managed scene, which is osgShadow::Shadowed scene */
    osg::ref_ptr<osg::Group>                        _scene;
    /*! \brief  Handle of the light implementation callback. See \ref setLightImplementationCallback */
    osg::observer_ptr<LightImplementationCallback>  _lightImplementationCallback;
    /*! \brief  The \ref igplugincore::PluginContext to pass Attributes, ex: igcore::FogAttributes
     *          to plugins */
    igplugincore::PluginContext                     _context;
    /*! \brief  Default script name. See \ref loadScript */
    std::string                                     _defaultScriptFileName;
    /*! \brief  ID based std::map of lights. See \ref addLight */
    LightsMap                                       _lights;
    /*! \brief  Root of the lights in the scene. This is passed in the \ref igcore::LightImplementationCallback */
    osg::ref_ptr<osg::Group>                        _lightsGroup;
    /*! \brief  Build-in Keypad Event Handler. See the onscreen help available on F7 for the keypad command */
    osg::ref_ptr<KeyPadEventHandler>                _keypad;
    /*! \brief  Build-in Keypad Camera manipulator. See the onscreen help available on F7 for the keypad command */
    osg::ref_ptr<KeyPadCameraManipulator>           _keypadCameraManipulator;
    /*! \brief  Handle of any CameraManipulator that was attached on the Viewer you provided in \ref init */
    osg::ref_ptr<osgGA::CameraManipulator>          _viewerCameraManipulator;
    /*! \brief  Set to true if there was no scene created with your Viewer */
    bool                                            _sceneCreatedByOpenIG;
    /*! \brief  If true with \ref setUpdateViewerCameraManipulator then the provided CameraManipulator is updated
     *          as well with any OpenIG Camera update method */
    bool                                            _updateViewerCameraMainpulator;
    /*! \brief  Handle of the splash screen */
    osg::ref_ptr<osg::Camera>                       _splashCamera;
    /*! \brief  Switch set to true when the splash screen is on */
    bool                                            _splashOn;
	/*! \brief	The root of the effects*/
	osg::ref_ptr<osg::Group>						_effectsRoot;
	/*! \brief	The effect implementation callback */
	osg::ref_ptr<igcore::GenericImplementationCallback>	_effectsImplementationCallback;

	/*! \brief	Our Effect*/
	typedef osg::ref_ptr<osg::MatrixTransform>			Effect;
	typedef std::map< unsigned int, Effect >			EffectMap;
	EffectMap											_effects;

	/*! \brief The Light Attributes */
	LightAttributesMap								_lightAttributes;

	/*! \brief The read file callback*/
	osg::ref_ptr<ReadNodeImplementationCallback>	_readFileCallback;

	/*! \brief The file cache map*/
	typedef std::map< std::string, osg::ref_ptr<osg::Node> >	EntityCache;
	EntityCache													_entityCache;

	/*! \brief The files to be cached*/
	igcore::StringUtils::StringList								_filesToBeCached;
	
    /*!
     * \brief Init the viewer. It calls \ref initScene and add the
     * ViewerOperation for managing the Entity maps. See \ref igcore::ImageGenerator::addEntity
     * \param viewer Instance of osgViewer::CompositeViewer
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initViewer(osgViewer::CompositeViewer* viewer);

    /*! Peforms init on the onscreen command line terminal. It is bound to F8
     * \brief Peforms init on the onscreen command line terminal
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initTerminal();

    /*! Performs init of commands. Here some default commands are implemented. There is
     * onscreen help where all the commands are listed with their usage, bound to F7
     * \brief Performs init of commands.
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initCommands();

    /*! Inits the internal \ref igplugincore::PluginContext. It only sets
     * the image generattor to this. See \ref igplugincore::PluginContext
     * \brief Inits the internal \ref igplugincore::PluginContext
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initPluginContext();

    /*! Init the default scene. See \ref igcore::ImageGenerator::getScene
     * \brief Init the scene
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initScene();

    /*! Init the onscreen help. It is bound to F7 where you can see all the available
     * commands and their usage
     * \brief Init the onscreen help
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */

    void initOnScreenHelp();
    /*! Init the splash screen. It looks for OpenIG-Splash.jpg in, for Windows in /igdata
     * Linux and MacOS in /usr/local/bin/igdata
     * \brief Init the splash screen
     * \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
     * \copyright (c)Compro Computer Services, Inc.
     * \date      Fri Jan 16 2015
     */
    void initSplashScreen();

	/*! Init the effects. Add the effects root to the scene
	* \brief Init the effects
	* \author    Trajce Nikolov Nick trajce.nikolov.nick@gmail.com
	* \copyright (c)Compro Computer Services, Inc.
	* \date      Sun Jun 14 2015
	*/
	void initEffects();

};

} // namespace

#endif // OPENIG_H
