// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   DigitCheck.cxx
/// \author Piotr Konopka
///

#include "EMCAL/DigitCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"
#include <fairlogger/Logger.h>
// ROOT
#include <TH1.h>
#include <TH2.h>
#include <TPaveText.h>
#include <TList.h>
#include <iostream>

using namespace std;

namespace o2::quality_control_modules::emcal
{

void DigitCheck::configure(std::string) {}

Quality DigitCheck::check(const MonitorObject* mo)
{
  std::cout << " Start Checking DigitCheck " << mo->getName() << std::endl;
  Quality result = Quality::Good;

  if (mo->getName() == "example") {
    auto* h = dynamic_cast<TH1F*>(mo->getObject());

    result = Quality::Good;

    for (int i = 0; i < h->GetNbinsX(); i++) {
      if (i > 0 && i < 8 && h->GetBinContent(i) == 0) {
        result = Quality::Bad;
        break;
      } else if ((i == 0 || i > 7) && h->GetBinContent(i) > 0) {
        result = Quality::Medium;
      }
    }
  }
  if (mo->getName() == "digitAmplitudeEMCAL") { //name of the checker. Or name of the histograms?cri.  HISTO
    auto* h = dynamic_cast<TH1*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
  }
  if (mo->getName() == "digitAmplitudeDCAL") {
    auto* h = dynamic_cast<TH1*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
  }
  if (mo->getName() == "digitAmplitudeHG") {
    auto* h = dynamic_cast<TH2*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
  }
  if (mo->getName() == "digitAmplitudeLG") {
    auto* h = dynamic_cast<TH2*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
  }

  if (mo->getName() == "digitTimeHG") {
    auto* h = dynamic_cast<TH2*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
    else {
      Float_t timeMean = h->GetMean();
      std::cout << " in the GetMean of digitTimeHG " << timeMean << std::endl;
      Float_t timeThrs = 60.;                                       //ns ? cri
      if (timeMean < timeThrs - 20. || timeMean > timeThrs + 20.) { //cri hard coded? 60ns, which units?

        std::cout << " out of range  of digitTimeHG " << timeMean << std::endl;
        result = Quality::Bad;
      }
    }
  }

  if (mo->getName() == "digitTimeLG") {
    auto* h = dynamic_cast<TH2*>(mo->getObject());
    if (h->GetEntries() == 0)
      result = Quality::Bad;
    else {
      Float_t timeMean = h->GetMean();
      Float_t timeThrs = 60;                                      //ns ? cri
      if (timeMean < timeThrs - 20 || timeMean > timeThrs + 20) { //cri hard coded? 60ns, which units?
        result = Quality::Bad;
      }
    }
  }
  std::cout << " finished check " << mo->getName() << std::endl;
  return result;
}

std::string DigitCheck::getAcceptedType() { return "TH1"; }

void DigitCheck::beautify(MonitorObject* mo, Quality checkResult)
{
  std::cout << " enter in Beautify in DigitCheck " << mo->getName() << std::endl;
  /*  if (mo->getName() == "example") {
    auto* h = dynamic_cast<TH1F*>(mo->getObject());

    if (checkResult == Quality::Good) {
      h->SetFillColor(kGreen);
    } else if (checkResult == Quality::Bad) {
      LOG(INFO) << "Quality::Bad, setting to red";
      h->SetFillColor(kRed);
    } else if (checkResult == Quality::Medium) {
      LOG(INFO) << "Quality::medium, setting to orange";
      h->SetFillColor(kOrange);
    }
    h->SetLineColor(kBlack);
  }
*/
  if (mo->getName().find("Time") != std::string::npos) {
    auto* h = dynamic_cast<TH2*>(mo->getObject());
    TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
    h->GetListOfFunctions()->Add(msg);
    msg->SetName(Form("%s_msg", mo->GetName()));

    if (checkResult == Quality::Good) {
      //
      msg->Clear();
      msg->AddText("Mean inside limits: OK!!!");
      msg->SetFillColor(kGreen);
      //
      h->SetFillColor(kGreen);
    } else if (checkResult == Quality::Bad) {
      LOG(INFO) << "Quality::Bad, setting to red";
      msg->Clear();
      msg->AddText("Mean outside limits or no entries");
      msg->AddText("If NOT a technical run,");
      msg->AddText("call EMCAL on-call.");
      h->SetFillColor(kRed);
    } else if (checkResult == Quality::Medium) {
      LOG(INFO) << "Quality::medium, setting to orange";
      h->SetFillColor(kOrange);
    }
    h->SetLineColor(kBlack);
  }
  if (mo->getName().find("Amplitude") != std::string::npos) {
    auto* h = dynamic_cast<TH1*>(mo->getObject());
    TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
    h->GetListOfFunctions()->Add(msg);
    msg->SetName(Form("%s_msg", mo->GetName()));

    if (checkResult == Quality::Good) {
      //
      msg->Clear();
      msg->AddText("Mean inside limits: OK!!!");
      msg->SetFillColor(kGreen);
      //
      h->SetFillColor(kGreen);
    } else if (checkResult == Quality::Bad) {
      LOG(INFO) << "Quality::Bad, setting to red";
      msg->Clear();
      msg->AddText("Mean outside limits or no entries");
      msg->AddText("If NOT a technical run,");
      msg->AddText("call EMCAL on-call.");
      h->SetFillColor(kRed);
    } else if (checkResult == Quality::Medium) {
      LOG(INFO) << "Quality::medium, setting to orange";
      h->SetFillColor(kOrange);
    }
    h->SetLineColor(kBlack);
  }

  std::cout << " exit from  Beautify in DigitCheck " << mo->getName() << std::endl;
}
} // namespace o2::quality_control_modules::emcal
