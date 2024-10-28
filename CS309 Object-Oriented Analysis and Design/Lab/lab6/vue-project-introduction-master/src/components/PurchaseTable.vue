<template>
  <div style="height: 100%">
    <el-row>
      <el-col :span="8" :offset="7">
        <p style="font-size: 40px">Purchase Table</p>
      </el-col>
      <el-col :span="3" :offset="2">
        <el-button type="primary" @click="addPurchaseForm"
                   style="margin-top: 50px; margin-left:90px;margin-bottom: 0px">
          Add
        </el-button>
      </el-col>
    </el-row>
    <el-col :span="17" :offset="3">
      <div v-if="isAddNewRecord">
        <el-form ref="form" :model="newRecord" label-width="80px">
          <el-form-item label="date">
            <el-input v-model="newRecord.date"></el-input>
          </el-form-item>
          <el-form-item label="description">
            <el-input v-model="newRecord.description"></el-input>
          </el-form-item>
          <el-form-item label="money">
            <el-input v-model="newRecord.money"></el-input>
          </el-form-item>
          <el-form-item label="type">
            <el-input v-model="newRecord.type"></el-input>
          </el-form-item>
          <el-form-item label="username">
            <el-input v-model="newRecord.username"></el-input>
          </el-form-item>
        </el-form>
        <el-button type="primary" @click="addPurchase" style="margin-top: 50px; margin-left:90px;margin-bottom: 0px">
          Finish
        </el-button>
        <el-button @click="returnPurchaseForm" style="margin-top: 50px; margin-left:90px;margin-bottom: 0px">
          Return
        </el-button>
      </div>

      <el-table
          v-if="!isAddNewRecord"
          :data="purchaseData"
          style="width: 100%; height: calc(100% - 30px);">
        <el-table-column v-for="(name, id) in purchaseColumn"
                         :key="name" :index="id"
                         fixed
                         :prop="name"
                         :label="name">
        </el-table-column>
        <!--        <el-table-column-->
        <!--            prop="price"-->
        <!--            label="Price">-->
        <!--        </el-table-column>-->
        <!--        <el-table-column-->
        <!--            prop="number"-->
        <!--            label="Number">-->
        <!--        </el-table-column>-->

        <el-table-column
            fixed="right"
            label=""
            width="100">
          <template slot-scope="scope">
            <el-button @click="deleteRow(scope.row)" type="text" size="middle">Delete</el-button>
          </template>
        </el-table-column>
      </el-table>
    </el-col>
  </div>
</template>

<script>
import {mapState} from "vuex";

export default {
  name: "PurchaseTable",
  data() {
    return {
      isAddNewRecord: false
    }
  },
  methods: {
    deleteRow(row) {
      console.log(row)
      this.$store.dispatch("purchase/deleteRecord", row.id)
    },
    addPurchase() {
      this.isAddNewRecord = false
      this.$store.dispatch("purchase/addRecord")
    },
    addPurchaseForm() {
      this.isAddNewRecord = true
    },
    returnPurchaseForm() {
      this.isAddNewRecord = false
    }
  },
  computed: {
    ...mapState('purchase', {
      purchaseData: state => state.rowData,
      purchaseColumn: state => state.purchaseColumns,
      newRecord: state => state.newRecord
    })
  },
  mounted() {
    this.$store.dispatch("purchase/listRecord")
  }
}
</script>

<style scoped>

</style>