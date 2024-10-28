<template>
  <el-form ref="form" :model="form" label-width="80px">
    <el-form-item label="Name">
      <el-input v-model="form.username"></el-input>
      <p class="login" v-if="isLoginClick && this.isUsernameEmpty">Empty Username!</p>
    </el-form-item>
    <el-form-item label="Password">
      <el-input v-model="form.password" type="password"></el-input>
      <p class="login" v-if="isLoginClick && isWrongPassword">Wrong Password or Valid Username</p>
    </el-form-item>
    <el-form-item>
      <el-button type="primary" @click="loginClick">Login</el-button>
      <el-button @click="registerClick">Register</el-button>
    </el-form-item>
  </el-form>
</template>

<script>

import {mapState} from "vuex";

export default {
  name: "LoginForm",
  props: ['form'],
  data() {
    return {
      isLoginClick: false
    }
  },
  methods: {
    loginClick() {
      this.$store.dispatch("purchase/loginCheck")
      this.isLoginClick = true
      if (!this.isWrongPassword) {
        this.$router.push('/purchase')
      }
    },
    registerClick() {
      this.$router.push('/register')
    }
  },
  computed: {
    ...mapState("purchase", {
      isWrongPassword: state => !state.accountValid
    }),
    isUsernameEmpty() {
      return this.form.username === null
    }
  },
  watch: {
    isWrongPassword() {
      if (!this.isWrongPassword) {
        this.$router.push('/purchase')
      }
    }
  }

}
</script>

<style scoped>
.login {
  margin-top: 10px;
  font-size: 14px;
  line-height: 22px;
  color: #dc3005;
  cursor: pointer;
  text-align: left;
  text-indent: 8px;
  width: 100%;
}
</style>