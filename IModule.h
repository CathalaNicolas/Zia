/**
 * \file      IModule.h
 * \version   1.0
 * \brief     Definit la classe abstraite IModule
 *
 * \details    La classe IModule est une interface qui doit etre implementer
 *			   par les modules.
 *
 */
#pragma once

#include "Event.h"
#include "Handler.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace ZiApi
{
	class Event;
	class Handler;

	class IModule : public std::enable_shared_from_this<IModule>
	{
	public:
		/**
		* \brief      Constructeur de la classe IModule
		* \details    Creer un IModule defini par un nom, une description et une version
		* \param	name			Nom du module
		* \param	description		Description du module
		* \param	version			Version du module
		* \return    Un \e IModule.
		*/
		IModule(const std::string &name="", const std::string &description="", const std::string &version="");
		virtual ~IModule();
		/**
		* \brief      Getter du nom du module
		* \details    Fonction donnant acces au nom du module
		*
		* \return    Une \e string correspondant au nom du module.
		 */
		std::string			getName()const;
		/**
		* \brief      Getter de la descritpion du module
		* \details    Fonction donnant acces a la description du module
		*
		* \return    Une \e string correspondant a la description du module.
		 */
		std::string			getDescription()const;
		/**
		* \brief      Getter de la version du module
		* \details    Fonction donnant acces a la version du module
		*
		* \return    Une \e string correspondant a la version du module.
		 */
		std::string			getVersion()const;
		/**
		* \brief      Ajoute un evenement pour le module.
		* \details    Fonction ajoutant un evenement pour le module
		* \param	event Un evenement
		 */
		void				addEvent(const Event &event);
		/**
		* \brief      Creer une liste des evenements du module.
		* \details   Creer une liste des evenements du module base sur le type
		*			 de leur point d'ancrage
		* \param	anchorPoint	Un point d'ancrage
		* \return    Une \e liste d'evenement.
		 */
		std::list<Event>	createListForAnchorPoint(const Event::AnchorPoint &anchorPoint)const;

	private:
		std::string			name;
		std::string			description;
		std::string			version;
		std::list<Event>	listEvent;
	};

}