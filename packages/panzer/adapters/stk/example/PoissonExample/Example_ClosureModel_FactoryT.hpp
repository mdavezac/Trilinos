#ifndef __Example_ClosureModelFactoryT_hpp__
#define __Example_ClosureModelFactoryT_hpp__

#include <iostream>
#include <sstream>
#include <typeinfo>
#include "Panzer_InputEquationSet.hpp"
#include "Panzer_IntegrationRule.hpp"
#include "Panzer_Basis.hpp"
#include "Panzer_Integrator_Scalar.hpp"
#include "Phalanx_FieldTag_Tag.hpp"
#include "Teuchos_ParameterEntry.hpp"
#include "Teuchos_TypeNameTraits.hpp"

// ********************************************************************
// ********************************************************************
template<typename EvalT>
Teuchos::RCP< std::vector< Teuchos::RCP<PHX::Evaluator<panzer::Traits> > > > 
Example::ModelFactory<EvalT>::
buildClosureModels(const std::string& model_id,
		   const panzer::InputEquationSet& set,
		   const Teuchos::ParameterList& models, 
		   const Teuchos::ParameterList& default_params,
		   const Teuchos::ParameterList& user_data,
		   const Teuchos::RCP<panzer::GlobalData>& global_data,
		   PHX::FieldManager<panzer::Traits>& fm) const
{
  using std::string;
  using std::vector;
  using Teuchos::RCP;
  using Teuchos::rcp;
  using Teuchos::ParameterList;
  using PHX::Evaluator;

  RCP< vector< RCP<Evaluator<panzer::Traits> > > > evaluators = 
    rcp(new vector< RCP<Evaluator<panzer::Traits> > > );

  if (!models.isSublist(model_id)) {
    models.print(std::cout);
    std::stringstream msg;
    msg << "Falied to find requested model, \"" << model_id 
	<< "\", for equation set:\n" << std::endl;
    TEUCHOS_TEST_FOR_EXCEPTION(!models.isSublist(model_id), std::logic_error, msg.str());
  }

  const ParameterList& my_models = models.sublist(model_id);

  for (ParameterList::ConstIterator model_it = my_models.begin(); 
       model_it != my_models.end(); ++model_it) {
    
    bool found = false;
    
    const std::string key = model_it->first;
    ParameterList input;
    const Teuchos::ParameterEntry& entry = model_it->second;
    const ParameterList& plist = Teuchos::getValue<Teuchos::ParameterList>(entry);

    if (plist.isType<double>("Value")) {
      { // at IP
	input.set("Name", key);
	input.set("Value", plist.get<double>("Value"));
	input.set("Data Layout", default_params.get<RCP<panzer::IntegrationRule> >("IR")->dl_scalar);
	RCP< Evaluator<panzer::Traits> > e = 
	  rcp(new panzer::Constant<EvalT,panzer::Traits>(input));
	evaluators->push_back(e);
      }
      { // at BASIS
	input.set("Name", key);
	input.set("Value", plist.get<double>("Value"));
	input.set("Data Layout", default_params.get<RCP<panzer::BasisIRLayout> >("Basis")->functional);
	RCP< Evaluator<panzer::Traits> > e = 
	  rcp(new panzer::Constant<EvalT,panzer::Traits>(input));
	evaluators->push_back(e);
      }
      found = true;
    }


    if (!found) {
      std::stringstream msg;
      msg << "ClosureModelFactory failed to build evaluator for key \"" << key 
	  << "\"\nin model \"" << model_id 
	  << "\".  Please correct the type or add support to the \nfactory." <<std::endl;
      TEUCHOS_TEST_FOR_EXCEPTION(!found, std::logic_error, msg.str());
    }

  }

  return evaluators;
}

#endif
