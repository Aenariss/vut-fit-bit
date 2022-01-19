from flask_security.forms import RegisterForm, Required
from wtforms import StringField, TextAreaField, SubmitField, PasswordField, DateField, SelectField
from datetime import datetime
from flask_wtf import FlaskForm 

class ExtendRegisterForm(RegisterForm):
    name = StringField('Jméno', [Required()])
    surname = StringField('Přijmení', [Required()])
    username = StringField('Uživatelské jmeno', [Required()])
    date = DateField('Date', default=datetime.now())
    

class NewCourse(FlaskForm):
    name = StringField('Name')
    shortcut = StringField('Shortcut')

class NewQuestion(FlaskForm):
    title = StringField('Title')
    content = TextAreaField('Content')

class NewCategory(FlaskForm):
    name = StringField('Name')

class NewResponse(FlaskForm):
    message = TextAreaField('Response')

class NewReply(FlaskForm):
    message = TextAreaField('Reply')