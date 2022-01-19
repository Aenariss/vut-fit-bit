def localize(app):

    app.config["SECURITY_MSG_USER_DOES_NOT_EXIST"] = ("Uživatel neexistuje!", "error")
    app.config["SECURITY_MSG_INVALID_PASSWORD"] = ("Neplatné heslo!", "error")
    app.config['SECURITY_MSG_PASSWORD_MISMATCH'] = ("Hesla se neshodují!", "error")
    app.config['SECURITY_MSG_RETYPE_PASSWORD_MISMATCH'] = ("Hesla se neshodují!", "error")
    app.config['SECURITY_MSG_EMAIL_ALREADY_ASSOCIATED'] = ("Tento email je již zaregistrován!", "error")
    app.config['SECURITY_MSG_REFRESH'] = ("Pro přístup se prosím znovu přihlaste", "error")
    app.config['SECURITY_MSG_LOGIN'] = ("Pro přístup ke stránce se prosím přihlašte", "error")
    app.config['SECURITY_MSG_INVALID_EMAIL_ADDRESS'] = ("Neplatná emailová adresa!", "error")
    app.config['SECURITY_MSG_EMAIL_NOT_PROVIDED'] = ("Nezadán email!", "error")
    app.config['SECURITY_MSG_PASSWORD_NOT_SET'] = ("Nezadáno heslo!", "error")

    return app