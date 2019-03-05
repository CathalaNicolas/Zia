/**
 * \file      Event.h
 * \version   1.0
 * \brief     Definit la classe Event
 *
 * \details    Cette classe permet aux modules de creer un evenement
 *             defini par un nom, une fonction a appeller, un point d'ancrage
 *				ainsi qu'une priorite.
 */
#pragma once

#include "HttpMessage.h"
#include <iostream>
#include <string>
#include <list>
#include <functional>

namespace ZiApi {

	class Handler;
	typedef std::function<void(std::shared_ptr<HttpMessage>&, std::shared_ptr<HttpMessage>&)> EventFunction;

	class Event
	{
	public:
		/**
		* \brief     Les differents point d'ancrage.
		* \details   Les points d'ancrage decrive quand est-ce qu'un evenement sera appele.
		*/
		enum AnchorPoint
		{
			REQUEST,			/*!< Point d'ancrage avant la recuperation de la requete*/
			AFTER_REQUEST,		/*!< Point d'ancrage apres la recuperation de la requete*/
			PARSING,			/*!< Point d'ancrage au parsing de la requete*/
			MODULE,				/*!< Point d'ancrage a la creation de la reponse*/
			BEFORE_RESPONSE,	/*!< Point d'ancrage avant l'envoi de la reponse*/
			RESPONSE			/*!< Point d'ancrage apres l'envoi de la reponse*/
		};

		/**
		* \brief     Les differentes priorite d'un evenement.
		* \details   La priorite d'un evenement determine sa place dans l'ordre d'appel des evenements.
		*/
		enum EventPriority
		{

			FIRST, /*!< Defini l'evenement comme l'un des premiers a appeler*/
			HIGH, /*!< Defini l'evenement avec une priorite grande*/
			MID, /*!< Defini l'evenement avec une priorite normale*/
			LOW, /*!< Defini l'evenement avec une priorite basse*/
		};

		/**
		* \brief      Constructeur de la classe Event.
		* \details    Creer un Event defini par un nom, une fonction a appeler, un point d'ancrage
		*			  ainsi qu'une priorite. 
		*
		* \param    name         Nom de l'evenement.
		* \param    function     Fonction a appeler.
		* \param	priority	 Priorite de l'evement enum \e EventPriority.
		* \param	anchorPoint	 Point d'ancrage de la fonction voir enum \e AnchorPoint.
		* \return    Un \e Event representant l'Event creer.
		 */
		Event(const std::string &name, const EventFunction &function, const EventPriority &priority, const AnchorPoint &anchorPoint);
		~Event();

		/**
		* \brief      Getter de la proriete de l'evenement.
		* \details    Fonction pour acceder a la priorite de l'evenement.
		*
		* \return	Un \e EventPriority voir l'enum \e EventPriority pour les valeurs.
		*/
		EventPriority									getPriority()const;

		/**
		* \brief      Getter du point d'ancrage de l'evenement.
		* \details    Fonction pour acceder au point d'ancrage de l'evenement.
		*
		* \return	Un \e AnchorPoint voir l'enum \e AnchorPoint pour les valeurs.
		*/
		AnchorPoint										getAnchorPoint()const;

		/**
		* \brief      Getter de la fonction liee a l'evenement.
		* \details    Fonction pour acceder a la fonction liee a l'evenement.
		*
		* \return	Un \e std::function<void(HttpMessage&, HttpMessage&)>
		*/
		EventFunction	getFunction()const;
		
	private:
		const bool			async_supported = false;
		const std::string	name;
		EventFunction		function;
		EventPriority		priority;
		AnchorPoint			anchorPoint;
	};

	/**
	* \brief      Compare la priorite de deux evenement.
	* \param	event1	Un evenement.
	* \param	event2	Un evenement.
	*
	* \return	\e True si event1 a une priorite superieur a event2, \e False sinon.
	*/
	bool	compareEvent(const Event &event1, const Event &event2);
}