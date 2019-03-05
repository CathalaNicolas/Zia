/**
 * \file      HttpMessage.h
 * \version   1.0
 * \brief     Definit la classe HttpMessage
 *
 * \details    Un HttpMessage contient le header ainsi que le body
 *			   d'une requete/reponse.
 *
 */
#pragma once

#include "HttpField.h"
#include <string>

namespace ZiApi
{
	class HttpMessage : public std::enable_shared_from_this<HttpMessage>
	{
	public:
		/**
		* \brief      Constructeur par defaut de la classe HttpMessage
		* \details    Creer un HttpMessage avec un header et un body vide
		* \return    Un \e HttpMessage.
		 */
		HttpMessage();
		/**
		* \brief      Constructeur de la classe HttpMessage
		* \details    Creer un HttpMessage avec un header et un body,
		*			  et parse les string pour recuperer les champs
		*			  et les valeurs
		*
		* \param    header		String correspondant au header
		* \param    function    String correspondant au body
		* \return    Un \e HttpMessage
		 */
		HttpMessage(const std::string &header, const std::string &body);
		/**
		* \brief      Constructeur de la classe HttpMessage
		* \details    Creer un HttpMessage avec un header et un body
		*
		* \param    header		Un HttpField correspondant au header
		* \param    body		Un HttpField correspondant au body
		* \return    Un \e HttpMessage
		 */
		HttpMessage(const std::shared_ptr<HttpField> &header, const std::shared_ptr<HttpField> &body);

		~HttpMessage();

		/**
		* \brief      Getter du header
		* \details    Fonction donnant acces a l'objet HttpField contenant
		*			  le headere
		*
		* \return    Un \e HttpField correspondant au header.
		 */
		std::shared_ptr<HttpField>	getHeader()const;
		/**
		* \brief      Getter du body
		* \details    Fonction donnant acces a l'objet HttpField contenant
		*			  le body
		*
		* \return    Un \e HttpField correspondant au body.
		 */
		std::shared_ptr<HttpField>	getBody()const;

		/**
		* \brief      Setter du Message
		* \details    Fonction initialisant le header ainsi que
		*			  le body du HttpMessage
		*\param message	Une string representant le message a parser
		 */
		void						setMessage(const std::string &message);

		void						setHeader(const std::string &header);
		void						setBody(const std::string &body);
		/**
		* \brief      Getter du Message
		* \details    Fonction retournant le Message
		*			  sous forme de string (header + body)
		*/
		std::string					getMessage()const;

	private:
		std::shared_ptr<HttpField>	header;
		std::shared_ptr<HttpField>	body;
	};

}