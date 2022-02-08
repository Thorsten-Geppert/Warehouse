#include "JasperReportsLibrary.h"
#include "JasperReportsEntities.h"
#include "JasperReportsEntity.h"
#include "../warehouse/JasperReportsSelectionDialog.h"

bool JasperReportsLibrary::DoReport(
	wxWindow *parent,
	RuntimeInformationType *rit,
	const wxString &name,
	const unsigned int id,
	JasperReportsController::TodoType todo
) {
	JasperReportsController jasperReportsController(rit);
	if(jasperReportsController.UseReports()) {
		JasperReportsEntities *jasperReportsEntities = jasperReportsController.GetReports(name);
		if(!jasperReportsEntities || jasperReportsEntities->GetCount() == 0) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				parent,
				_("There are no available reports"),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		} else {
			JasperReportsEntity *jasperReportsEntity = NULL;
			if(jasperReportsEntities->GetCount() > 1) {
				JasperReportsSelectionDialog *jasperReportsSelectionDialog = new JasperReportsSelectionDialog(
					parent,
					-1,
					rit,
					*jasperReportsEntities
				);
				jasperReportsSelectionDialog->Center();
				if(jasperReportsSelectionDialog->ShowModal() == wxID_OK)
					jasperReportsEntity = jasperReportsSelectionDialog->GetSelectedEntry();
				jasperReportsSelectionDialog->Destroy();
			} else {
				jasperReportsEntity = jasperReportsEntities->Get(0);
			}

			if(jasperReportsEntity) {
				JasperReportsController::ErrorType error = jasperReportsController.DoReport(
					jasperReportsEntity->Get(_N("report"))->ToString(),
					id,
					todo
				);

				if(error != JasperReportsController::DONE) {
					wxMessageDialog *errorMessage = new wxMessageDialog(
						parent,
						jasperReportsController.GetErrorDescription(error),
						_("Error")
					);
					errorMessage->Center();
					errorMessage->ShowModal();
					errorMessage->Destroy();
				}
			}
		}

		wxDELETE(jasperReportsEntities);

		return true;
	}

	return false;
}

bool JasperReportsLibrary::UseReports(RuntimeInformationType *rit) {
	JasperReportsController jasperReportsController(rit);
	return jasperReportsController.UseReports();
}
