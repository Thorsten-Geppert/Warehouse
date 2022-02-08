#ifndef REMINDER_TYPE_H_
#define REMINDER_TYPE_H_

#include <DSDateTime.h>

using namespace DynSoft;

class ReminderType {

	public:
		enum Types {
			CLIENTS = 0,
			PRODUCTS,
			ORDERS,
			VOUCHERS,
			PURCHASES
		};

	public:
		ReminderType();
		ReminderType(
			const unsigned int id,
			const wxString &username,
			const unsigned int module,
			const unsigned int reference,
			const wxString &description,
			const DSDateTime &dateTime,
			const wxString &notes
		);

		void SetId(const unsigned int id);
		unsigned int GetId() const;

		void SetUsername(const wxString &username);
		wxString GetUsername() const;

		void SetModule(const unsigned int module);
		unsigned int GetModule() const;

		void SetReference(const unsigned int reference);
		unsigned int GetReference() const;

		void SetDescription(const wxString &description);
		wxString GetDescription() const;

		void SetDateTime(const DSDateTime &dateTime);
		DSDateTime GetDateTime() const;

		void SetNotes(const wxString &notes);
		wxString GetNotes() const;

		wxString ToString();

		wxString GetModuleString();
		static wxString GetModuleString(const unsigned int module);

	protected:
		unsigned int id;
		wxString username;
		unsigned int module;
		unsigned int reference;
		wxString description;
		DSDateTime dateTime;
		wxString notes;

};

#endif /* REMINDER_TYPE_H_ */
