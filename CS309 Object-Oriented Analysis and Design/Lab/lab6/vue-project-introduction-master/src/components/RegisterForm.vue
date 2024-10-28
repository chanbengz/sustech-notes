<template>
  <el-form ref="form" :model="form" label-width="80px">
    <el-form-item>
      <el-input v-model="form.username" auto-complete="off" placeholder="User name"></el-input>
    </el-form-item>
    <el-form-item>
      <el-input v-model="form.emailAddress" auto-complete="off" placeholder="Email"></el-input>
    </el-form-item>
    <el-form-item class="code">
      <el-input v-model="form.verifyCode" placeholder="Verification Code"></el-input>
      <el-button type="primary" :disabled='isDisable' @click="sendVerifyCode">Send Verify Code
      </el-button>
    </el-form-item>
    <el-form-item>
      <el-input v-model="form.password" auto-complete="off" placeholder="Password"></el-input>
    </el-form-item>
    <el-form-item>
      <el-input v-model="form.confirmPassword" auto-complete="off" placeholder="Confirm Password"></el-input>
      <p class="warn" v-if="!isPWDSame">The confirmed password should be same with the password!</p>
      <p class="warn" v-if="isRegisterClick && errorMsg!== null">{{ errorMsg }}</p>

    </el-form-item>
    <el-form-item>
      <el-button type="primary" @click="registerClick" style="width:100%;">Register</el-button>
      <p class="login" @click="gotoLogin">Already have an account? Log in immediately </p>
    </el-form-item>
  </el-form>

</template>

<script>
import {mapState} from "vuex";

export default {
  name: "RegisterForm",
  computed: {
    ...mapState("purchase", {
      form: state => state.form,
      accountValid: state => state.accountValid,
      errorMsg: state => state.errorMsg
    }),
    isPWDSame() {
      return this.form.password === this.form.confirmPassword
    }
  },
  data() {
    return {
      isDisable: false,
      isRegisterClick: false,
    }
  },
  methods: {
    sendVerifyCode() {
    },
    gotoLogin() {
      this.$router.push('/login')
    },
    registerClick() {
      this.isRegisterClick = true

      if (this.isPWDSame) {
        this.$store.dispatch("purchase/registerAccount")
      }
    }
  },
  watch: {
    accountValid() {
      if (this.accountValid) {
        this.$router.push('/purchase')
      }
    },
  }
}
</script>

<style scoped>
.code >>> .el-form-item__content {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.code button {
  margin-left: 20px;
  width: 140px;
  text-align: center;
}

.login {
  margin-top: 10px;
  font-size: 14px;
  line-height: 22px;
  color: #1ab2ff;
  cursor: pointer;
  text-align: left;
  text-indent: 8px;
  width: 100%;
}

.warn {
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