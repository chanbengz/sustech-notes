<template>
  <div id="main-pane">
    <el-row class="boundary" style="width: 100%; height: calc(30% - 4px); overflow: hidden">
      <div class="titletext">
        <p>OOAD Vue Introduction Project</p>
      </div>
    </el-row>

    <el-row class="box" style="height: 100%">
      <PurchaseTable></PurchaseTable>
    </el-row>
  </div>

</template>github

<script>
import PurchaseTable from "@/components/PurchaseTable";

export default {
  name: "Purchase",
  components: {
    PurchaseTable
  },
  data() {
    return {}
  },
  computed: {},
  methods: {}
}
</script>

<style scoped>
.box {
  /*border-style: dashed;*/
  border-style: solid;
  border-color: #d3dce6;
  border-width: 2px;
  border-radius: 10px;
  margin-top: 20px;
  height: 100%;
}
</style>