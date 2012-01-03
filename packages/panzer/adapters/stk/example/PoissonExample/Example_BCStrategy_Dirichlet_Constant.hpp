#ifndef __Example_BC_Dirichlet_Constant_hpp__
#define __Example_BC_Dirichlet_Constant_hpp__

#include <vector>
#include <string>

#include "Teuchos_RCP.hpp"
#include "Panzer_BCStrategy_Dirichlet_DefaultImpl.hpp"
#include "Panzer_Traits.hpp"
#include "Panzer_PureBasis.hpp"
#include "Phalanx_FieldManager.hpp"

namespace Example {

  template <typename EvalT>
  class BCStrategy_Dirichlet_Constant : public panzer::BCStrategy_Dirichlet_DefaultImpl<EvalT> {
  public:    
    
    BCStrategy_Dirichlet_Constant(const panzer::BC& bc, const Teuchos::RCP<panzer::GlobalData>& global_data);
    
    void setup(const panzer::PhysicsBlock& side_pb,
	       const Teuchos::ParameterList& user_data);
    
    void buildAndRegisterEvaluators(PHX::FieldManager<panzer::Traits>& fm,
				    const panzer::PhysicsBlock& pb,
				    const panzer::ClosureModelFactory_TemplateManager<panzer::Traits>& factory,
				    const Teuchos::ParameterList& models,
				    const Teuchos::ParameterList& user_data) const;

    std::string residual_name;
    Teuchos::RCP<panzer::PureBasis> basis;
  };

}

#include "Example_BCStrategy_Dirichlet_ConstantT.hpp"

#endif
