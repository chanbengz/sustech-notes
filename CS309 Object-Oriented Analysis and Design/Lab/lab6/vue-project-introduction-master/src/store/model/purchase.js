import dataService from "@/service/dataService";

const state = () => ({
    purchaseColumns: ["date", "description", "money", "type", "username"],

    rowData: null,

    newRecord: {
        "date": "",
        "description": "",
        "money": "",
        "type": null,
        "username": ""
    },

    idToRecord: {},

    form: {
        "username": null,
        "password": null,
        "confirmPassword": null,
        "emailAddress": null,
        "verifyCode": null
    },
    accountValid: false,
    errorMsg: null
})

const actions = {
    loginCheck(context) {
        dataService.loginCheck(context.state.form, resp => {
            if (resp.data.code === 0) {
                context.commit("changeAccountStatus", true)
            }
        })
    },

    registerAccount(context) {
        console.log(context.state.form)
        dataService.registerAccount({
            "username": context.state.form.username,
            "password": context.state.form.password,
            "confirmPassword": context.state.form.confirmPassword,
        }, resp => {
            console.log(resp)
            if (resp.data.code === 0) {
                context.commit("changeAccountStatus", true)
            } else {
                context.state.errorMsg = resp.data.msg
            }
        })
    },

    listRecord(context) {
        dataService.listRecord(
            {}, resp => {
                console.log(resp)
                if (resp.data.code === 0) {
                    context.commit("updateRowData", resp.data.data)
                } else {
                    context.state.errorMsg = resp.data.msg
                }
            }
        )
    },

    addRecord(context) {
        dataService.addRecord(context.state.newRecord, resp => {
            console.log("add record", resp)
            if (resp.data.code === 0) {
                context.dispatch("listRecord")
                // context.commit("addRecord", context.state.newRecord)
            } else {
                context.state.errorMsg = resp.data.msg
            }
        })
    },

    deleteRecord(context, id) {
        dataService.deleteRecord(id, resp => {
            console.log(resp)
            if (resp.data.code === 0) {
                context.commit("deleteRecord", id)
            }
        })
    }


}
const mutations = {
    changeAccountStatus(state, status) {
        state.accountValid = status
    },
    updateRowData(state, data) {
        state.rowData = data
    },
    addRecord(state, data) {
        state.rowData.push(data)
    },
    deleteRecord(state, id) {
        let tmp = []
        for (let i = 0; i < state.rowData.length; i++) {
            if (state.rowData[i].id !== id) {
                tmp.push(state.rowData[i])
                // state.rowData.splice(id, 1)
                // break
            }
        }
        state.rowData = tmp
    }
}


export default {
    namespaced: true,
    state,
    actions,
    mutations
}