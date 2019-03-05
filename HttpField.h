/**
 * \file      HttpField.h
 * \version   1.0
 * \brief     Definit la classe HttpField
 *
 * \details    Un HttpField defini une partie d'une requete/reponse HTTP,
 *			   comme un header, un body ou autre.       
 *				
 */
#pragma once

#include <string>
#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <regex>

namespace ZiApi
{
	class HttpField
	{
	public:
		/**
		* \brief      Constructeur par d�faut de la classe HttpField
		* \details    Cr�er un HttpField vide.
		*
		* \return    Un \e HttpField.
		*/
		HttpField() {};
		/**
		* \brief      Constructeur de la classe HttpField.
		* \details    Cr�er un HttpField
		*
		* \param    field   Une string correspondant a la partie de la requ�te/r�ponse
							que l'on souhaite cr��r.
		* \return    Un \e HttpField.
		*/
		HttpField(const std::string &field);
		~HttpField();
		/**
		* \brief      Getter de la map de valeurs de la requ�te/r�ponse.
		* \details    Fonction donnant acc�s a la map de valeurs du HttpField.
		*			  
		*
		* \return    Une \e map<std::string, std::string> de valeur.
		*/
		std::map<std::string, std::string>	getField()const;
		/**
		* \brief      Retourne la valeur d'un champs bas� sur sa clef.
		* \param	key	Une string correspondant a un champs de la requ�te/r�ponse.
		*
		* \return    Une \e string correspondant a la valeur de la clef.
		*/
		std::string							getValueFromMapField(const std::string &key)const;
		
		/**
		* \brief    Setter de la partie de la requ�te/r�ponse.
		* \details	Fonction modifiant la partie de la requ�te/r�ponse, la fonction parse
		*			la string pour r�cup�rer les champs et leurs valeurs.
		* \param    field   Une string correspondant a la partie de la requ�te/r�ponse
							que l'on souhaite cr��r.
		*/
		void								setField(const std::string &field);
		/**
		* \brief    Retourne la string enti�re non pars�e.
		*
		* \return    Une \e string correspondant a la partie de la requ�te/r�ponse non pars�e.
		*/
		std::string							getStringField()const { return field; }

	private:
		std::string							field;
		std::map<std::string, std::string>	mapField;

		void								mapMethodValue(const std::string &value);
		void								mapStringField(const std::string &field);
	};

}