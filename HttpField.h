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
		* \brief      Constructeur par défaut de la classe HttpField
		* \details    Créer un HttpField vide.
		*
		* \return    Un \e HttpField.
		*/
		HttpField() {};
		/**
		* \brief      Constructeur de la classe HttpField.
		* \details    Créer un HttpField
		*
		* \param    field   Une string correspondant a la partie de la requête/réponse
							que l'on souhaite créér.
		* \return    Un \e HttpField.
		*/
		HttpField(const std::string &field);
		~HttpField();
		/**
		* \brief      Getter de la map de valeurs de la requête/réponse.
		* \details    Fonction donnant accés a la map de valeurs du HttpField.
		*			  
		*
		* \return    Une \e map<std::string, std::string> de valeur.
		*/
		std::map<std::string, std::string>	getField()const;
		/**
		* \brief      Retourne la valeur d'un champs basé sur sa clef.
		* \param	key	Une string correspondant a un champs de la requête/réponse.
		*
		* \return    Une \e string correspondant a la valeur de la clef.
		*/
		std::string							getValueFromMapField(const std::string &key)const;
		
		/**
		* \brief    Setter de la partie de la requête/réponse.
		* \details	Fonction modifiant la partie de la requête/réponse, la fonction parse
		*			la string pour récupérer les champs et leurs valeurs.
		* \param    field   Une string correspondant a la partie de la requête/réponse
							que l'on souhaite créér.
		*/
		void								setField(const std::string &field);
		/**
		* \brief    Retourne la string entière non parsée.
		*
		* \return    Une \e string correspondant a la partie de la requête/réponse non parsée.
		*/
		std::string							getStringField()const { return field; }

	private:
		std::string							field;
		std::map<std::string, std::string>	mapField;

		void								mapMethodValue(const std::string &value);
		void								mapStringField(const std::string &field);
	};

}