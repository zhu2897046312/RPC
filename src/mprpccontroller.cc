#include "mprpccontroller.h"

namespace fst {

MprpcController::MprpcController(){
    m_failed = false;
    m_errText = "";
}

void MprpcController::Reset(){
    m_failed = false;
    m_errText = "";
}

bool MprpcController::Failed() const{
   return m_failed;
}

std::string MprpcController::ErrorText() const{
    return m_errText;
}

void MprpcController::SetFailed(const std::string& reason){
    m_failed = true;
    m_errText = reason;
} 

}