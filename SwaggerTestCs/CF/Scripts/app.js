import {campoBase, condicionalCampo} from './functions.js';
campoBase([{
  'url': 'e-forms/circularizacaoGestores_V2.csv',
  'idCampo': [
    {
      'ID':'data-gestor',
      'position': 0
    }
  ]
}]);

condicionalCampo([{
  'elementEvent': 'publico-alvo',
  'elementCondicionado': 'minuta',
  'valueCompare': 'geral'
}]);


               
//Doc Launcher Call
                var url = 'https://apiuatna11.springcm.com/v201411/doclaunchertasks';
				var params = '<Parameters><ContractID>' + escapeSpecialChars($("#contractId").val()) + '</ContractID>';
                params+= '<SupplierName>' + escapeSpecialChars($("#supplier_name").val()) + '</SupplierName>';
                params += '<SupplierID>' + escapeSpecialChars($("#supplier_id").val()) + '</SupplierID>';
                params += '<SupplierContactName>' + escapeSpecialChars($("#supplier_contact_name").val()) + '</SupplierContactName>';
                params += '<SupplierContactEmail>' + escapeSpecialChars($("#supplier_contact_email").val()) + '</SupplierContactEmail>';
                params += '<ParentAgreement>' + escapeSpecialChars($("#parent_agreement").val()) + '</ParentAgreement>';
                params += '<Description>' + escapeSpecialChars($("#description").val()) + '</Description>';
                params += '<BmrnNumber>' + escapeSpecialChars($("#bmrn_entity").val()) + '</BmrnNumber>';
                params += '<BmrnDescription>' + escapeSpecialChars($("#bmrn_description").val()) + '</BmrnDescription>';
                params += '<RequestingDepartmentNumber>' + escapeSpecialChars($("#requesting_department").val()) + '</RequestingDepartmentNumber>';
                params += '<RequestingDepartmentDescription>' + escapeSpecialChars($("#requesting_department_description").val()) + '</RequestingDepartmentDescription>';
                params += '<ContractRequestType>' + escapeSpecialChars($(".RequestType").val())/*$("#__sxform_2ab94c24-a128-e911-9c1d-3ca82a1e3f41_sxform__").val()*/ + '</ContractRequestType>';
                params += '<ContractCategory>' + escapeSpecialChars($(".ContractCategory").val())/*$("#__sxform_ddb2652c-cd3b-e911-9c1d-3ca82a1e3f41_sxform__").val()*/ + '</ContractCategory>';
                params += '<ContractType>' + escapeSpecialChars($(".ContractType").val())/*$("#__sxform_2cb94c24-a128-e911-9c1d-3ca82a1e3f41_sxform__").val()*/ + '</ContractType>';
                params += '<TemplateType>' + escapeSpecialChars($(".TemplateName").val())/*$("#__sxform_2eb94c24-a128-e911-9c1d-3ca82a1e3f41_sxform__").val()*/ + '</TemplateType>';
                params += '<ContractAmount>' + escapeSpecialChars($("#contract_amount").val()) + '</ContractAmount>';
                params += '<CurrencyUnit>' + escapeSpecialChars($(".CurrencyUnit").val())/*$("#__sxform_df66231f-60d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</CurrencyUnit>';
                params += '<PaymentTerms>' + escapeSpecialChars($(".PaymentTerms").val())/*$("#__sxform_ea66231f-60d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</PaymentTerms>';
                params += '<DeliveryTerms>' + escapeSpecialChars($(".DeliveryTerms").val())/*$("#__sxform_ee66231f-60d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</DeliveryTerms>';
                params += '<EffectiveDate>' + escapeSpecialChars($("#effective_date").val()) + '</EffectiveDate>';
                params += '<ExpirationDate>' + escapeSpecialChars($("#expiration_date").val()) + '</ExpirationDate>';
                params += '<BusinessOwnerName>' + escapeSpecialChars($("#business_owner_name").val()) + '</BusinessOwnerName>';
                params += '<BusinessOwnerEmail>' + escapeSpecialChars($("#business_owner_email").val()) + '</BusinessOwnerEmail>';
                params += '<RequireAccess>' + escapeSpecialChars($(".RequireAccess").val())/*$("#__sxform_65d2ca73-5fd9-e811-9c19-3ca82a1e3f41_sxform__").val()*/+ '</RequireAccess>';
                params += '<ItElement>' + escapeSpecialChars($(".ItElement").val())/*$("#__sxform_68d2ca73-5fd9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</ItElement>';
                params += '<SupplierProvider>' + escapeSpecialChars($(".HealthcareProvider").val())/*$("#__sxform_6bd2ca73-5fd9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</SupplierProvider>';
                params += '<Confidential>' + escapeSpecialChars($(".Confidential").val())/*$("#__sxform_6ed2ca73-5fd9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</Confidential>';
                params += '<SpecialCertification>' + escapeSpecialChars($(".SpecialCertification").val())/*$("#__sxform_71d2ca73-5fd9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</SpecialCertification>';
                params += '<GovtOfficial>' + escapeSpecialChars($(".GovtOfficial").val())/*$("#__sxform_98289fc1-57d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</GovtOfficial>';
                params += '<DescribeMaterial>' + escapeSpecialChars($(".DescribeMaterial").val())/*$("#__sxform_e4857349-3d89-e911-9c1f-d89d6716196d_sxform__").val()*/ + '</DescribeMaterial>';
                params += '<ClinicalProgram>' + escapeSpecialChars($(".ClinicalProgramAssociation").val())/*$("#__sxform_92289fc1-57d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</ClinicalProgram>';
                params += '<PrincipalInvestigator>' + escapeSpecialChars($(".PrincipalInvestigator").val())/*$("#__sxform_98289fc1-57d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</PrincipalInvestigator>';
                params += '<ProgramNumberOld>' + escapeSpecialChars($("#program_number").val()) + '</ProgramNumberOld>';
                params += '<ProgramNumber>' + escapeSpecialChars($(".ProgramNumber").val())/*$("#__sxform_fdb9c145-575d-e911-9c1f-d89d6716196d_sxform__").val()*/ + '</ProgramNumber>';
                params += '<ProtocolNumber>' + escapeSpecialChars($(".ProtocolNumber").val())/*$("#__sxform_99289fc1-57d9-e811-9c19-3ca82a1e3f41_sxform__").val()*/ + '</ProtocolNumber>';            
                params += '<userName>' + escapeSpecialChars(document.getElementById("RequestorUserName").value) + '</userName>';
                params += '<userEmail>' + escapeSpecialChars(document.getElementById("RequestorEmail").value) + '</userEmail>';
                params += '<userFullName>' + escapeSpecialChars(document.getElementById("RequestorName").value) + '</userFullName>';
                params += '<isSelfAuthoredDropdown>' + escapeSpecialChars(document.getElementsByClassName('SelfAuthoredDropdown').value) + '</isSelfAuthoredDropdown>'
                params += '<ContractAssociate>' + escapeSpecialChars($('#contractAssociate').val()) + '</ContractAssociate>'
                params += '<RequisitionNumber>' + escapeSpecialChars($('.RequisitionNumber').val()) + '</RequisitionNumber>'
                params += '</Parameters>';
                params = params.replace(/&/g, '&amp;');
				console.log("Paramas passed to DL:" + params);
        var body = '{
          "Data": "' + params + '",
          "DataType": "xml",
          "DestinationFolder": {
            "Href": "https://apiuatna11.springcm.com/v201411/folders/' + destinationFolderId + '"
          },
          "DocLauncherConfiguration": {
            "Href":"' + dlconfigurl + dlconfigID + '"
          }
        }';
				

                //API CALL
				SpringCM.API.post(url, body, function (_data) {
					if (document.getElementById("contractAssociate").value == ""){
						window.location.replace(_data.DocLauncherResultUrl);
					}
					else{

                     window.open(_data.DocLauncherResultUrl);
                        $(".SendButton").click();
					}
				});