//
//    File: FTCalCluster.h
// Created: Wed Jan  8 12:49:31 CET 2020
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Jefferson Science Associates LLC Copyright Notice:  
// Copyright 251 2014 Jefferson Science Associates LLC All Rights Reserved. Redistribution
// and use in source and binary forms, with or without modification, are permitted as a
// licensed user provided that the following conditions are met:  
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.  
// 3. The name of the author may not be used to endorse or promote products derived
//    from this software without specific prior written permission.  
// This material resulted from work developed under a United States Government Contract.
// The Government retains a paid-up, nonexclusive, irrevocable worldwide license in such
// copyrighted data to reproduce, distribute copies to the public, prepare derivative works,
// perform publicly and display publicly and to permit others to do so.   
// THIS SOFTWARE IS PROVIDED BY JEFFERSON SCIENCE ASSOCIATES LLC "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
// JEFFERSON SCIENCE ASSOCIATES, LLC OR THE U.S. GOVERNMENT BE LIABLE TO LICENSEE OR ANY
// THIRD PARTES FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#ifndef _TriggerDecision_h_
#define _TriggerDecision_h_

#include <JANA/JObject.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
/// This holds the result of a certain Trigger Decision, that is here implemented as a (tagged) factory.
/// In this way, I can design the code jtridas to get ALL trigger decisions (ideally, one per tagged factory),
/// and then determine what to do, without having to know how many trigger decisions will be implemented.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class TriggerDecision: public JObject {
public:
	TriggerDecision():decision(false){}
	TriggerDecision(std::string val):decision(false),description(val){}
	virtual ~TriggerDecision(){}

	uint16_t GetDecision() const{return decision;};
	void SetDecision(uint16_t val=1){decision=val;};

	std::string GetDescription()const{return description;};
	void SetDescription(std::string val){description=val;};

	uint16_t GetID() const{return id;};
	void SetID(uint32_t newid){ id = newid; };

protected:

private:
	uint16_t decision;
	std::string description;
	uint16_t id=0;
};

#endif // _TriggerDecision_h_

