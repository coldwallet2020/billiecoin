// Copyright (c) 2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "governance-types.h"

GovernanceObjectType GovernanceStringToType(std::string strType) {
    GovernanceObjectType type = Error;

    if(strType == "AllTypes") type = AllTypes;
    if(strType == "Error") type = Error;
    if(strType == "ValueOverride") type = ValueOverride;
    if(strType == "BilliecoinNetwork") type = BilliecoinNetwork;
    if(strType == "BilliecoinNetworkVariable") type = BilliecoinNetworkVariable;
    if(strType == "Category") type = Category;
    if(strType == "Group") type = Group;
    if(strType == "User") type = User;
    if(strType == "Company") type = Company;
    if(strType == "Project") type = Project;
    if(strType == "ProjectReport") type = ProjectReport;
    if(strType == "ProjectMilestone") type = ProjectMilestone;
    if(strType == "Proposal") type = Proposal;
    if(strType == "Contract") type = Contract;

    return type;
};

std::string GovernanceTypeToString(GovernanceObjectType type) {
    std::string s = "";

    if(type == AllTypes) s = "AllTypes";
    if(type == Error) s = "Error";
    if(type == ValueOverride) s = "ValueOverride";
    if(type == BilliecoinNetwork) s = "BilliecoinNetwork";
    if(type == BilliecoinNetworkVariable) s = "BilliecoinNetworkVariable";
    if(type == Category) s = "Category";
    if(type == Group) s = "Group";
    if(type == User) s = "User";
    if(type == Company) s = "Company";
    if(type == Project) s = "Project";
    if(type == ProjectReport) s = "ProjectReport";
    if(type == ProjectMilestone) s = "ProjectMilestone";
    if(type == Proposal) s = "Proposal";
    if(type == Contract) s = "Contract";

    return s;
};