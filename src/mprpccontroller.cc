#include "mprpccontroller.h"

namespace fst {

MprpcConTroller::MprpcConTroller(){
    m_failed = false;
    m_errText = "";
}

void MprpcConTroller::Reset(){
    m_failed = false;
    m_errText = "";
}

bool MprpcConTroller::Failed() const{
   return m_failed;
}

std::string MprpcConTroller::ErrorText() const{
    return m_errText;
}

void MprpcConTroller::SetFailed(const std::string& reason){
    m_failed = true;
    m_errText = reason;
} 

}