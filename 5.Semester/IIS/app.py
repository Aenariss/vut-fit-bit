# VUT FIT IIS Projekt 2021
# Autori: Vojtech Fiala, Adam Policek, David Svaty

from flask import Flask, render_template, redirect, url_for, request, session
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_security import Security, SQLAlchemyUserDatastore, UserMixin, RoleMixin, current_user, login_required
from datetime import datetime, timedelta
from wtforms.validators import InputRequired, Length, ValidationError, DataRequired, Regexp
from sqlalchemy import or_
from flask_security.forms import Required, RegisterForm
from sqlalchemy.sql import func
import secrets

import logging
import re

from src.inputFields import *
from src.utils import *
from src.localization import *

######################
# APP INICIALIZATION #
######################
app = Flask(__name__)

app = localize(app)

app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://adminiis:IISpassw1234@iis-db.mysql.database.azure.com/iis?charset=utf8mb4'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['DEBUG'] = True
app.config['SECRET_KEY'] = 'mysecret'
app.config['SECURITY_REGISTERABLE'] = True
app.config['SECURITY_PASSWORD_SALT'] = 'randomstufftosaltpasswith'
app.config['SECURITY_SEND_REGISTER_EMAIL'] = False

db = SQLAlchemy(app)
migrate = Migrate(app, db)
#########################
# END OF INICIALIZATION #
#########################

##########################################
# CLASSES BASED ON DB TABLES DEFINITIONS #
##########################################

class UserRoles(db.Model):
    __tablename__ = "UserRoles"
    user_id = db.Column(db.Integer(), db.ForeignKey("user.id", ondelete='CASCADE'), primary_key=True)
    role_id = db.Column(db.Integer(), db.ForeignKey("role.id", ondelete='CASCADE'))    

class Role(db.Model, RoleMixin):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), unique=True)
    description = db.Column(db.String(250))

class Upvote(db.Model):
    user = db.Column(db.Integer, primary_key=True)
    reply = db.Column(db.Integer, primary_key=True)
    question = db.Column(db.Integer)
    value = db.Column(db.Integer)

class course_registration(db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    course = db.Column(db.String(10), db.ForeignKey('course.shortcut', ondelete='CASCADE'))
    user = db.Column(db.Integer, db.ForeignKey('user.id', ondelete='CASCADE'))
    approved = db.Column(db.Boolean)

class Course(db.Model):
    name = db.Column(db.String(100))
    grade = db.Column(db.Integer)
    shortcut = db.Column(db.String(10), primary_key=True)
    author = db.Column(db.Integer, db.ForeignKey('user.id', ondelete='SET NULL'))
    date = db.Column(db.DateTime())
    approved = db.Column(db.Boolean)

class Category(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100))
    course = db.Column(db.String(10), db.ForeignKey('course.shortcut', ondelete='CASCADE'))

class Years(db.Model):
    year = db.Column(db.String(4), primary_key=True)    

class Reply(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    question = db.Column(db.Integer, db.ForeignKey('question.id', ondelete='CASCADE'))
    author = db.Column(db.Integer, db.ForeignKey('user.id', ondelete='SET NULL'))
    message = db.Column(db.String(600))
    date = db.Column(db.DateTime())
    upvote = db.Column(db.Integer)
    solution = db.Column(db.Boolean, default=False)
    correct = db.Column(db.Boolean, default=False)

class Question(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    title = db.Column(db.String(100),nullable=False)
    body = db.Column(db.String(600),nullable=False)
    date = db.Column(db.DateTime())
    author = db.Column(db.Integer,db.ForeignKey('user.id', ondelete='SET NULL'))
    course = db.Column(db.String(10),db.ForeignKey('course.shortcut', ondelete='CASCADE'))
    year = db.Column(db.String(4),db.ForeignKey('years.year', ondelete='CASCADE'))
    lastUpdated = db.Column(db.DateTime())
    solved = db.Column(db.Boolean)
    category = db.Column(db.String(100))

class Response(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    reply = db.Column(db.Integer,db.ForeignKey('reply.id', ondelete='CASCADE'))
    author = db.Column(db.Integer,db.ForeignKey('user.id', ondelete='SET NULL'))
    question = db.Column(db.Integer, db.ForeignKey('question.id', ondelete='CASCADE'))
    date = db.Column(db.DateTime())
    message = db.Column(db.String(300),nullable=False)


class Visited(db.Model):
    page = db.Column(db.String(255))
    user = db.Column(db.Integer, db.ForeignKey('user.id', ondelete='CASCADE'), primary_key=True)
    visited = db.Column(db.DateTime())

class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    email = db.Column(db.String(255), unique=True)
    name = db.Column(db.String(255))
    password = db.Column(db.String(255))
    surname = db.Column(db.String(255))
    username = db.Column(db.String(255), unique=True)
    date = db.Column(db.DateTime, default=func.now())
    roles = db.relationship('Role', secondary='UserRoles', viewonly=True)
    active = db.Column(db.Boolean)
    karma = db.Column(db.Integer, default=0)

class ExtendRegisterForm(RegisterForm):
    name = StringField('Jméno', validators=[DataRequired(), Length(min=2, max=20), Regexp('[A-Za-z]+')])
    surname = StringField('Přijmení', validators=[DataRequired(), Length(min=2, max=20), Regexp('[A-Za-z]+')])
    username = StringField('Uživatelské jméno', validators=[DataRequired(), Length(min=4, max=20), Regexp('[0-9A-Za-z_]+')])

    def validate_username(self, username):
        user = User.query.filter_by(username=username.data).first()
        if user:
            raise ValidationError('Vybraná přezdívka je již zabraná.')

############################
# END OF CLASS DEFINITIONS #
############################


# Create register and login pages
user_datastore = SQLAlchemyUserDatastore(db, User, Role)
security = Security(app, user_datastore, register_form=ExtendRegisterForm)    

# Logout the user automatically after inactivity period
# To do so we first make the session "permanent"
# and afterwards change the "permanent" to 20 minutes
# https://stackoverflow.com/a/11785722/13279982
@app.before_request
def make_session_permanent():
    session.permanent = True
    app.permanent_session_lifetime = timedelta(minutes=20)

# Title page with grades rendering
@app.route('/', methods=['GET', 'POST'])
def index():

    dt = datetime.today()
    month = dt.month
    status = 0

    # Check if it's time to show the Move Up a Year button
    if (month >= 5 and month <= 9):
        status = 1
    curr_roles = []
    if (current_user.is_authenticated):
        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())

    courses = Course.query.all()    
    curr_year = Years.query.all()

    # Find current school year to correctly redirect to newest year in courses on titlepage
    if (curr_year != []):
        curr_year = curr_year[len(curr_year)-1].year
    else:
        curr_year = 0

    return render_template('index.html', current_user=current_user, roles=curr_roles, status=status, courses=courses, year=curr_year)

# Function to move up a year, aka go from 2021/2022 to 2022/2023
@app.route('/add_new_year', methods=["POST"])
def add_new_year():

    if not (is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    years = Years.query.all()

    max_year = 0
    for year in years:
        if int(year.year) > max_year:
            max_year = int(year.year) 
    
    new_year = max_year+1
    new_year_to_db = Years(year=new_year)
    db.session.add(new_year_to_db)
    db.session.commit()

    return redirect(url_for('index'))

# Function to search questions based on their title or body, shown as the searchbar
@app.route('/search_questions', methods=["GET"])
def search_questions():
    search_text = None
    if (request.args.get('searchbar') != None):
        search_text = '%' + request.args.get('searchbar') + '%'
    else:
        return bad_request(400)
    results = Question.query.filter(or_(Question.title.ilike(search_text), Question.body.ilike(search_text))).all()
    validate_results = []

    # Make sure questions from deleted courses are not there
    for result in results:
        course = Course.query.filter_by(shortcut=result.course).first()
        if course == None or course.approved == False:
            continue
        else:
            validate_results.append(result)
    
    author_username, author_id = get_authors(User, validate_results)
    author_role, author_karma = get_authors_role_karma(User, Role, UserRoles, validate_results)

    return render_template('search_questions.html', results=validate_results, author_username=author_username, author_id=author_id, author_role=author_role, author_karma=author_karma)

# User Profile
@app.route('/profile/<author>', methods=['GET', 'POST'])
def profile(author):
    user = User.query.filter_by(id=author).first()
    status = 0
    curr_roles = ['student']

    if (user == None):
        return page_not_found(404)

    # Editing buttons
    if current_user.is_authenticated:
        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())
        if "admin" in curr_roles or current_user.id == user.id:
            if (request.args.get("editmail") != None):
                if (re.search('^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$', request.args.get("editmail"))):
                    if len(request.args.get("editmail")) > 255:
                        status = 4
                    else:
                        user.email = request.args.get("editmail")
                        db.session.commit()
                else:
                    status = 1
            
            elif (request.args.get("editusername") != None):
                if (User.query.filter_by(username=request.args.get("editusername")).first() != None):
                    status = 2
                else:
                    if len(request.args.get("editusername")) > 255:
                        status = 4
                    else:
                        user.username = request.args.get("editusername")
                        db.session.commit()
            
            elif (request.args.get("editkarma") != None):
                if not is_admin(current_user, Role, UserRoles):
                    pass
                else:
                    if not (re.search("^[0-9]+$", request.args.get("editkarma"))):
                        status = 3
                    else:
                        try:
                            if (int(request.args.get("editkarma")) > 2147483647 or int(request.args.get("editkarma")) < 0):
                                status = 3
                            else:
                                user.karma = request.args.get("editkarma")
                                db.session.commit()
                        except:
                            status = 3

    if not user.date:   # development only
        user.date = datetime.now()
        db.session.commit()

    role_names = get_role_names(Role, UserRoles.query.filter_by(user_id=user.id).all())
    return render_template('profile.html', user=user, current_user=current_user, role_names=role_names, curr_roles=curr_roles, status=status)

# User courses page
@app.route('/user_courses/<userid>', methods=['GET'])
def my_courses(userid):
    
    username = User.query.get(userid)

    if username == None:
        return page_not_found(404)

    username = username.username

    courses = None
    roles = ['student']

    if (current_user.is_authenticated):
        if (str(current_user.id) == userid) or is_admin(current_user, Role, UserRoles):
            courses = Course.query.filter_by(author=userid).all()
            roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())

        else:
            courses = Course.query.filter_by(author=userid, approved=True).all()
    else:
        courses = Course.query.filter_by(author=userid, approved=True).all()

    new_years = Years.query.all()
    new_yrs = get_years(new_years)

    if (courses == None):
        return render_template('my_courses.html', courses=courses, username=username, years=new_yrs, fcurrent_user=current_user,
                            unread_courses=None, new_years=None, roles=roles, id=userid)

    new_courses, unread_years = tmp_new_courses, tmp_unread_years = get_unread_courses(current_user, db, courses, Question, Reply, Response, Visited, '/1bit', new_yrs, status=1)
    
    grades = ['/2bit', '/3bit', '/1mit', '/2mit']
    for grade in grades:
        tmp_new_courses, tmp_unread_years = get_unread_courses(current_user, db, courses, Question, Reply, Response, Visited, grade, new_yrs, status=1)
        new_courses.append(tmp_new_courses)
        unread_years.append(tmp_unread_years)

    return render_template('my_courses.html', courses=courses, username=username, years=new_yrs, fcurrent_user=current_user,
                            unread_courses=new_courses, new_years=unread_years, roles=roles, id=userid)

# User questions page
@app.route('/user_questions/<userid>', methods=['GET'])
def my_questions(userid):

    username = User.query.get(userid)

    if (username == None):
        return page_not_found(404)

    username = username.username

    roles = ['student']
    questions = None

    if (current_user.is_authenticated):
        if (str(current_user.id) == userid) or is_admin(current_user, Role, UserRoles) or is_mod(current_user, Role, UserRoles):
            questions = Question.query.filter_by(author=userid).all()
        else:
            questions = get_approved_questions(userid, Question, Course)

    else:
        questions = get_approved_questions(userid, Question, Course)

    if (questions == None):
        return render_template('my_questions.html', questions=questions, username=username, new_questions=None, courseshrts=None, id=userid)

    i = 0
    new_questions = [0] * len(questions)
    if not (current_user.is_authenticated):
        new_questions = [1] * len(questions)
        return render_template('my_questions.html', questions=questions, username=username, new_questions=new_questions, id=userid)

    for question in questions:

        replies = Reply.query.filter_by(question=question.id).all()
        responses = Response.query.filter_by(question=question.id).all()
        tmp_path = "/forum/" + str(question.course) + '/' + str(question.year) + '/' + str(question.id)
        tmp_last_visit = get_last_visit(current_user, Visited, tmp_path)
        new_questions = compare_dates_overwrite(replies, tmp_last_visit, new_questions, i)
        new_questions = compare_dates_overwrite(responses, tmp_last_visit, new_questions, i)
        i -=- 1

    return render_template('my_questions.html', questions=questions, username=username, new_questions=new_questions, id=userid)

# Delete user button functionality
@app.route('/delete_user/<userid>', methods=['POST'])
@login_required
def delete_user(userid):

    if not (str(userid) == str(current_user.id) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    user = User.query.get(userid)


    if (user == None):
        return page_not_found(404)

    # Don't use delete cascade on user, cuz we don't want to delete his questions, responses etc...
    # Deleting only the "support" tables records
    roles = UserRoles.query.filter_by(user_id=userid).all()
    for role in roles:
        db.session.delete(role)

    upvotes = Upvote.query.filter_by(user=userid).all()
    for upvote in upvotes:
        db.session.delete(upvote)

    visitations = Visited.query.filter_by(user=userid).all()
    for visit in visitations:
        db.session.delete(visit)

    course_registrations = course_registration.query.filter_by(user=userid).all()
    for reg in course_registrations:
        db.session.delete(reg)

    # And finally delete the user himself
    # In the courses & stuff he'll be showed as "DELETED USER"
    db.session.delete(user)
    db.session.commit()

    return redirect(url_for('index'))

# Function to make an each year page with their corresponding courses (together with course accepting & deleting)
def bitTemplate(page_name, grade, title):

    form = NewCourse()
    status = 0

    accepted_courses = Course.query.filter_by(grade=grade, approved=True).all()
    not_acc_courses = Course.query.filter_by(grade=grade, approved=False).all()

    author_username, author_id = get_authors(User, accepted_courses)
    author_role, author_karma = get_authors_role_karma(User, Role, UserRoles, accepted_courses)

    author_username_not_acc , author_id_not_acc = get_authors(User, not_acc_courses)
    author_role_not_acc, author_karma_not_acc = get_authors_role_karma(User, Role, UserRoles, not_acc_courses)

    new_years = Years.query.all()
    new_yrs = get_years(new_years)

    new_courses, unread_years = get_unread_courses(current_user, db, accepted_courses, Question, Reply, Response, Visited, page_name, new_yrs)
    new_courses_not_acc, unread_years_2 = get_unread_courses(current_user, db, not_acc_courses, Question, Reply, Response, Visited, page_name, new_yrs)

    curr_roles = []
    if (current_user.is_authenticated):
        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())

    if form.validate_on_submit():
        if not (current_user.is_authenticated):
            return unauthorized(401)
        if (len(form.name.data) > 100 or len(form.name.data) == 0 or grade > 5 or grade < 1 or len(form.shortcut.data) > 10 or len(form.shortcut.data) == 0):
            status = 2
            return render_template('1bit.html', status=status, title=title, form=form, 
                new_years=unread_years, new_years_2=unread_years_2, unread_courses=new_courses, courses=accepted_courses, 
                fcurrent_user=current_user, years=new_yrs, author_username=author_username, author_id=author_id, author_role=author_role, author_karma=author_karma,
                unread_courses_not_acc=new_courses_not_acc, not_acc_courses=not_acc_courses,
                author_username_not_acc=author_username_not_acc, author_id_not_acc=author_id_not_acc, author_role_not_acc=author_role_not_acc, author_karma_not_acc=author_karma_not_acc,
                roles=curr_roles)
        else:
            # Course shortcut MUST be unique
            if (Course.query.filter_by(shortcut=form.shortcut.data).first() != None):
                status = 1
                return render_template('1bit.html', status=status, title=title, form=form, 
                new_years=unread_years, new_years_2=unread_years_2, unread_courses=new_courses, courses=accepted_courses, 
                fcurrent_user=current_user, years=new_yrs, author_username=author_username, author_id=author_id, author_role=author_role, author_karma=author_karma,
                unread_courses_not_acc=new_courses_not_acc, not_acc_courses=not_acc_courses,
                author_username_not_acc=author_username_not_acc, author_id_not_acc=author_id_not_acc, author_role_not_acc=author_role_not_acc, author_karma_not_acc=author_karma_not_acc,
                roles=curr_roles)

            new_course = Course(name=form.name.data, grade=grade, shortcut=form.shortcut.data, author=current_user.id, date=datetime.now(), approved=False)
            update_last_visit(current_user, Visited, page_name)
            db.session.add(new_course)
            db.session.commit()

        if (grade == 1):
            return redirect(url_for('firstBIT'))
        elif (grade == 2):
            return redirect(url_for('secondBIT'))
        elif (grade == 3):
            return redirect(url_for('thirdBIT'))
        elif (grade == 4):
            return redirect(url_for('firstMIT'))    
        elif (grade == 5):
            return redirect(url_for('secondMIT'))

    return render_template('1bit.html', status=status, title=title, form=form, 
    new_years=unread_years, new_years_2=unread_years_2, unread_courses=new_courses, courses=accepted_courses, 
    fcurrent_user=current_user, years=new_yrs, author_username=author_username, author_id=author_id, author_role=author_role, author_karma=author_karma,
    unread_courses_not_acc=new_courses_not_acc, not_acc_courses=not_acc_courses, 
    author_username_not_acc=author_username_not_acc, author_id_not_acc=author_id_not_acc, author_role_not_acc=author_role_not_acc, author_karma_not_acc=author_karma_not_acc,
    roles=curr_roles)

# Functions to call the BIT template that makes the grade page
@app.route('/1bit', methods=['GET', 'POST'])
def firstBIT():

    page_name = "/1bit"
    grade = 1
    title = "1BIT"

    return bitTemplate(page_name, grade, title)

@app.route('/2bit', methods=['GET', 'POST'])
def secondBIT():

    page_name = "/2bit"
    grade = 2
    title = "2BIT"

    return bitTemplate(page_name, grade, title)

@app.route('/3bit', methods=['GET', 'POST'])
def thirdBIT():

    page_name = "/3bit"
    grade = 3
    title = "3BIT"

    return bitTemplate(page_name, grade, title)

@app.route('/1mit', methods=['GET', 'POST'])
def firstMIT():

    page_name = "/1mit"
    grade = 4
    title = "1MIT"

    return bitTemplate(page_name, grade, title)

@app.route('/2mit', methods=['GET', 'POST'])
def secondMIT():

    page_name = "/2mit"
    grade = 5
    title = "2MIT"

    return bitTemplate(page_name, grade, title)

# Function to create course page with questions
@app.route('/forum/<courseshrt>/<year>', methods=['GET', 'POST'])
def question(courseshrt, year):

    curr_roles = []
    if (current_user.is_authenticated):
        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())

    year_exists = Years.query.get(year)
    if (year_exists == None):
        return page_not_found(404)

    course_exists = Course.query.filter_by(shortcut=courseshrt).all()
    if (course_exists == []):
        return page_not_found(404)

    else:
        if (course_exists[0].approved == False):
            if (not 'admin' in curr_roles and not 'mod' in curr_roles):
                if (current_user.is_authenticated):
                    if (current_user.id != course_exists[0].author):
                            return unauthorized(401)
                else:
                    return unauthorized(401)
    
    form = NewQuestion()
    page_name = "/forum/" + courseshrt + '/' + year
    questions = Question.query.filter_by(course=courseshrt, year=year).all()
    err_status = 0

    if (questions == None):
        return page_not_found(404)

    if form.validate_on_submit():

        if (request.form.get("roles") != None):
            if (len(request.form.get("roles")) > 100 or request.form.get("roles") == 0):
                err_status = 1

        if (len(form.title.data) > 100 or len(form.title.data) == 0 or len(form.content.data) > 600 or len(form.content.data) == 0):
            err_status = 1

        err_mark = 1
        categories = Category.query.filter_by(course=courseshrt).all()
        if categories == []:
            err_mark = 0
        else:
            for category in categories:
                if request.form.get("roles") == category.name:
                    err_mark = 0
            if (err_mark == 1):
                return bad_request(400)
            
        if (err_status != 1):
            new_question = Question(category=request.form.get("roles"), title=form.title.data, lastUpdated=datetime.now(), body=form.content.data, course=courseshrt, year=year, date=datetime.now(), author=current_user.id, solved=False)
            update_last_visit(current_user, Visited, page_name) 
            db.session.add(new_question)
            db.session.commit()
            return redirect(url_for('question', year=year, courseshrt=courseshrt))
    
    last_visit = get_last_visit_with_update(Visited, current_user, page_name, db)
    new_questions = get_unread_questions(current_user, Question, Reply, Response, Visited, courseshrt, year, last_visit)

    status = -1
    if (current_user.is_authenticated):
        check_if_reg = course_registration.query.filter_by(user=current_user.id, course=course_exists[0].shortcut).first()
        if (check_if_reg != None):
            if (check_if_reg.approved == True):
                status = 2
            else:
                status = 1
        else:
            status = 0

    categories = Category.query.filter_by(course=course_exists[0].shortcut).all()

    author_username, author_id = get_authors(User, questions)
    author_role, author_karma = get_authors_role_karma(User, Role, UserRoles, questions)
    course_title = courseshrt + ' ' + str(int(year)-1) + '/' + year

    return render_template('forum_temp.html', roles=curr_roles, course=course_exists[0], status=status, categories=categories, title=course_title, new_questions=new_questions, form=form, 
        questions=questions, err_status=err_status, fcurrent_user=current_user,year=year,courseshrt=courseshrt, username=author_username, author_id=author_id, author_role=author_role, author_karma=author_karma)

# Function to register for course
@login_required
@app.route('/register_course/<courseshrt>/<userid>/<year>', methods=['POST'])
def register_for_course(courseshrt, userid, year):
    course = Course.query.filter_by(shortcut=courseshrt).first()

    if (course == None):
        return bad_request(400)

    user = User.query.get(userid)
    if (user == None):
        return bad_request(400)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return bad_request(400)

    check_if_reg = course_registration.query.filter_by(user=userid, course=course.shortcut).first()
    if (check_if_reg == None):
        register_user_for_course = course_registration(course=course.shortcut, user=userid, approved=False)
        db.session.add(register_user_for_course)
        db.session.commit()

    return redirect(url_for('question', courseshrt=courseshrt, year=year))

# Function to set course categories
@login_required
@app.route('/forum/<courseshrt>/<year>/set_categories', methods=['GET', 'POST'])
def set_categories(courseshrt, year):

    if not (is_course_author(current_user, Course, courseshrt) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    course_id = Course.query.filter_by(shortcut=courseshrt).first()

    if (course_id == None):
        return bad_request(400)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return bad_request(400)
    
    form = NewCategory()
    status = 0

    if (form.validate_on_submit()):
        if (len(form.name.data) > 100 or len(form.name.data) == 0):
            status = 1
        else:
            new_categories = Category(name=form.name.data, course=course_id.shortcut)
            db.session.add(new_categories)
            db.session.commit() 
            return redirect(url_for('set_categories', courseshrt=courseshrt, year=year))

    categories = Category.query.filter_by(course=course_id.shortcut).all()

    return render_template('set_categories.html', courseshrt=courseshrt, categories=categories, form=form, year=year, status=status)

# Function to create page with course applicants and approve/disapprove them
@login_required
@app.route('/forum/<courseshrt>/<year>/check_applicants', methods=['GET', 'POST'])
def check_applicants(courseshrt, year):

    if not (is_course_author(current_user, Course, courseshrt) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    course_id = Course.query.filter_by(shortcut=courseshrt).first()

    if (course_id == None):
        return bad_request(400)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return bad_request(400)
    
    form = NewCategory()

    unreg_users = course_registration.query.filter_by(approved=False, course=course_id.shortcut).all()
    reg_users = course_registration.query.filter_by(approved=True, course=course_id.shortcut).all()

    unreg_names = get_user_names(User, unreg_users)
    reg_names = get_user_names(User, reg_users)

    return render_template('check_applicants.html', year=year, courseshrt=courseshrt, unreg_users=unreg_users, reg_users=reg_users, unreg_names=unreg_names, reg_names=reg_names)

# Function to approve or reject a course applicant
@login_required
@app.route('/approve_user/<courseshrt>/<userid>/<status>/<year>', methods=['POST'])
def approve_user(courseshrt, userid, status, year):

    if not (is_course_author(current_user, Course, courseshrt) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    course = Course.query.filter_by(shortcut=courseshrt).first()
    approve_user = course_registration.query.filter_by(user=userid, course=course.shortcut).first()

    if (course == None):
        return bad_request(400)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return bad_request(400)

    if (approve_user == None):
        return bad_request(400)

    if (status == '1'):
        approve_user.approved = True
    else:
        db.session.delete(approve_user)
    db.session.commit()
    
    return redirect(url_for('check_applicants', courseshrt=courseshrt, year=year))

# Function to delete a category
@login_required
@app.route('/delete_category/<courseshrt>/<categoryid>/<year>', methods=['POST'])
def delete_category(courseshrt, categoryid, year):

    category_to_delete = Category.query.filter_by(id=categoryid).first()

    if (category_to_delete == None):
        return bad_request(400)
    
    course = Course.query.filter_by(shortcut=courseshrt).first()

    if (course == None):
        return bad_request(400)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return bad_request(400)

    if not (is_course_author(current_user, Course, courseshrt) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    db.session.delete(category_to_delete)
    db.session.commit()
    
    return redirect(url_for('set_categories', courseshrt=courseshrt, year=year))

# Function to make a page with question and its responses & replies (aka its thread)
@app.route('/forum/<courseshrt>/<year>/<questionid>', methods=['GET', 'POST'])
def thread(courseshrt,year,questionid):


    already_replied = 0
    status = -1

    course = Course.query.filter_by(shortcut=courseshrt).first()
    if (course == None):
        return page_not_found(404)

    curr_roles = ['student']

    if (course.approved == False):
        if (current_user.is_authenticated):
            curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())
            if not ("admin" in curr_roles or "mod" in curr_roles or course.author == current_user.id):
                return unauthorized(401)
        else:
            return unauthorized(401)

    year_tst = Years.query.get(year)
    if (year_tst == None):
        return page_not_found(404)

    if (current_user.is_authenticated):
        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())
        replies = Reply.query.filter_by(question=questionid, author=current_user.id).all()
        if len(replies) > 0:
            already_replied = 1

        check_if_reg = course_registration.query.filter_by(user=current_user.id, course=course.shortcut).first()
        if (check_if_reg != None):
            if (check_if_reg.approved == True):
                status = 2
            else:
                status = 1
        else:
            if not (course.author == current_user.id or "admin" in curr_roles):
                status = 0


    form = NewReply()
    question = Question.query.get(int(questionid))
    if (question == None):
        return page_not_found(404)

    q_author = User.query.get(question.author)

    q_author_username = None
    q_author_karma = 0
    q_author_role = ['student']
    q_author_id = -1

    if (q_author != None):
        q_author_username = q_author.username
        q_author_role = get_role_names(Role, UserRoles.query.filter_by(user_id=q_author.id).all())
        q_author_karma = q_author.karma
        q_author_id = q_author.id

    # Urceni casu posledni navstevy (pokud vubec nekdy)
    page_name = "/forum/" + courseshrt + '/' + year + '/' + questionid
    last_visit = get_last_visit_with_update(Visited, current_user, page_name, db)
    # Konec urceni casu posledni navstevy
    err_status = 0

    if form.validate_on_submit():
        if (status == 1 or status == 0):
            return unauthorized(401)

        elif (len(form.message.data) > 600 or len(form.message.data) == 0):
            err_status = 3

        else:
            if (str(q_author_id) != str(current_user.id) or str(current_user.id) == str(course.author) or is_admin(current_user, Role, UserRoles)):  # If current user is question author, he cant answer
                new_reply = Reply(message=form.message.data, question=questionid, date=datetime.now(), upvote=0, author=current_user.id, solution=False)
                update_last_visit(current_user, Visited, page_name)    # Fix, at to novy reply nezobrazuje autorovi jako "novy"
                question.lastUpdated = datetime.now()
                db.session.add(new_reply)
                db.session.commit()
                return redirect(url_for('thread', course=course, courseshrt=courseshrt, year=year, questionid=questionid, _anchor=new_reply.id))

    replies = Reply.query.filter_by(question=questionid, solution=False).all()

    final_replies = Reply.query.filter_by(question=questionid, solution=True).all()

    rep_author_username, rep_author_role, rep_author_karma, rep_ids, rep_upvotes, rep_author_ids = get_reply_author_info(replies, current_user, User, Role, UserRoles, Upvote)
    fin_rep_author_username, fin_rep_author_role, fin_rep_author_karma, fin_rep_ids, fin_rep_upvotes, fin_rep_author_ids = get_reply_author_info(final_replies, current_user, User, Role, UserRoles, Upvote)

    responses = Response.query.filter_by(question=questionid).all() 
    fin_responses = []
    resp_author_username = []
    final_resp_author_username = []
    resp_author_role = []
    resp_author_karma = []
    resp_author_ids = []
    final_resp_author_ids = []
    final_resp_author_role = []
    final_resp_author_karma = []

    for response in responses:
        tmp_rep = Reply.query.filter_by(id=response.reply).first()
        author = User.query.get(response.author)
        if tmp_rep != None:
            if (tmp_rep.solution == True):
                if (author == None):
                    final_resp_author_username.append("DELETED USER")
                    final_resp_author_role.append(['None'])
                    final_resp_author_karma.append(0)
                    final_resp_author_ids.append(-1)
                else:
                    final_resp_author_username.append(author.username)
                    final_resp_author_role.append(get_role_names(Role, UserRoles.query.filter_by(user_id=author.id).all()))
                    final_resp_author_karma.append(author.karma)
                    final_resp_author_ids.append(author.id)
                fin_responses.append(response)
                continue
            else:
                if (author == None):
                    resp_author_username.append("DELETED USER")
                    resp_author_role.append(['None'])
                    resp_author_karma.append(0)
                    resp_author_ids.append(-1)
                else:
                    resp_author_username.append(author.username)
                    resp_author_role.append(get_role_names(Role, UserRoles.query.filter_by(user_id=author.id).all()))
                    resp_author_karma.append(author.karma)
                    resp_author_ids.append(author.id)
        else:
            continue

    # Urceni, ktere responses jsou nove
    new_mark_responses = compare_dates(responses, last_visit)
    fin_new_mark_responses = compare_dates(fin_responses, last_visit)
    # Konec urcovani aktuality responses
    # Urceni, ktere replies jsou nove
    new_mark_replies = compare_dates(replies, last_visit)
    fin_new_mark_replies = compare_dates(final_replies, last_visit)
    # Konec urcovani aktuality replies
    return render_template('thread_temp.html', roles=curr_roles, course=course, status=status, already_replied=already_replied, question=question, 
    visited_responses=new_mark_responses, visited_replies=new_mark_replies, form=form, replies=replies, responses=responses ,fcurrent_user=current_user, 
    courseshrt=courseshrt, year=year, q_author_username=q_author_username, q_author_id=q_author_id, q_author_role=q_author_role, q_author_karma=q_author_karma,
    rep_author_username=rep_author_username, rep_author_role=rep_author_role, rep_author_karma=rep_author_karma,
    resp_author_username=resp_author_username, resp_author_role=resp_author_role, resp_author_karma=resp_author_karma,
    fin_replies=final_replies, fin_responses=fin_responses, fin_rep_author_username=fin_rep_author_username, fin_rep_author_role=fin_rep_author_role,
    fin_rep_author_karma=fin_rep_author_karma, fin_rep_ids=fin_rep_ids, fin_resp_author_username=final_resp_author_username, fin_resp_author_role=final_resp_author_role,
    fin_resp_author_karma=final_resp_author_karma, fin_visited_responses=fin_new_mark_responses, fin_visited_replies=fin_new_mark_replies,
    rep_upvotes=rep_upvotes, fin_rep_upvotes=fin_rep_upvotes, err_status=err_status, rep_author_ids=rep_author_ids, fin_rep_author_ids=fin_rep_author_ids,
    resp_author_ids=resp_author_ids, final_resp_author_ids=final_resp_author_ids)

# https://stackoverflow.com/a/42888467/13279982
# Debug function -> Call from jinja as {{ mdebug(Value) }} and it'll get printed in the console
@app.context_processor
def utility_functions():
    def print_in_console(message):
        print (str(message))

    return dict(mdebug=print_in_console)

#delete course 
@login_required
@app.route('/delete_course/<coursegrade>/<courseid>', methods=['POST'])
def delete_course(courseid,coursegrade):
    
    if not (is_course_in_grade(courseid, coursegrade, Course)):
        return bad_request(400)

    course_to_delete = Course.query.filter_by(shortcut=courseid).first()

    if not (is_author(current_user, course_to_delete) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)
    # Also dont forget to delete all questions, replies and responses   -> ON DELETE CASCADE
    db.session.delete(course_to_delete)
    db.session.commit()

    if coursegrade == '1':
        return redirect(url_for('firstBIT'))
    elif coursegrade == '2':
        return redirect(url_for('secondBIT'))
    elif coursegrade == '3':
        return redirect(url_for('thirdBIT'))
    else:
        return redirect(url_for('index'))

# approve course (only for admins & mods)
@login_required
@app.route('/approve_course/<coursegrade>/<courseid>/<status>', methods=['GET'])
def approve_course(courseid, coursegrade, status):

    if not (status in ['1', '0'] and is_course_in_grade(courseid, coursegrade, Course)):
        return bad_request(400)

    if not (is_mod(current_user, Role, UserRoles) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    course_to_approve = Course.query.filter_by(shortcut=courseid).first()
    if (status == '1'):
        course_to_approve.approved = True
    else:
        course_to_approve.approved = False
    db.session.commit()

    if coursegrade == '1':
        return redirect(url_for('firstBIT'))
    elif coursegrade == '2':
        return redirect(url_for('secondBIT'))
    elif coursegrade == '3':
        return redirect(url_for('thirdBIT'))
    elif coursegrade == '4':
        return redirect(url_for('firstMIT'))
    elif coursegrade == '5':
        return redirect(url_for('secondMIT'))
    else:
        return redirect(url_for('index'))

#delete question 
@login_required
@app.route('/delete_question/<courseshrt>/<year>/<questionid>', methods=['POST'])
def delete_question(questionid, courseshrt, year):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question)):
        return bad_request(400)

    question_to_delete = Question.query.filter_by(id=questionid).first()

    if not (is_author(current_user, question_to_delete) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    db.session.delete(question_to_delete)
    db.session.commit()
    return redirect(url_for('question',courseshrt=courseshrt,year=year))

#delete reply
@login_required
@app.route('/delete_reply/<courseshrt>/<year>/<questionid>/<replyid>', methods=['POST'])
def delete_reply(replyid, courseshrt,year, questionid):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and is_reply_valid(replyid, Reply, questionid)):
        return bad_request(400)

    reply_to_delete = Reply.query.filter_by(id=replyid).first()

    if not (is_author(current_user, reply_to_delete) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    db.session.delete(reply_to_delete)
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt, year=year, questionid=questionid))

#delete response
@login_required
@app.route('/delete_response/<courseshrt>/<year>/<questionid>/<replyid>/<respid>', methods=['POST'])
def delete_response(respid,courseshrt,year,questionid,replyid):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and is_reply_valid(replyid, Reply, questionid) and is_response_valid(respid, Response, replyid)):
        return bad_request(400)

    response_to_delete = Response.query.filter_by(id=respid).first()

    if not (is_author(current_user, response_to_delete) or is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    db.session.delete(response_to_delete)
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt, year=year, questionid=questionid, _anchor=str(replyid)))

#toggle question as solved 
@login_required
@app.route('/toggle_solved/<courseshrt>/<year>/<questionid>/<status>', methods=['POST'])
def toggle_solved(questionid, courseshrt, year, status):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and (status == '0' or status == '1')):
        return bad_request(400)

    if (not (is_course_author(current_user, Course, courseshrt)) and not is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    question_to_mark = Question.query.filter_by(id=questionid).first()
    if (status == '1'):
        question_to_mark.solved = True;
        # add karma to user profile
        upvotes_under_question = Upvote.query.filter_by(question=questionid).all()
        for upvote in upvotes_under_question:
            db.session.delete(upvote)
    else:
        question_to_mark.solved = False;
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt,year=year, questionid=questionid))

#mark reply as solution
@login_required
@app.route('/mark_reply_solved/<courseshrt>/<year>/<questionid>/<replyid>/<status>', methods=['POST'])
def mark_reply_solved(courseshrt,year,questionid,replyid,status):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and (status == '0' or status == '1') and is_reply_valid(replyid, Reply, questionid)):
        return bad_request(400)

    if (not (is_course_author(current_user, Course, courseshrt)) and not is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    reply_to_mark = Reply.query.filter_by(id=replyid).first()
    already_solution = Reply.query.filter_by(question=questionid, solution=True).first()
    if not already_solution:
        if(status == '1'):
            reply_to_mark.solution = True
            user = User.query.filter_by(id=reply_to_mark.author).first()
            if (user != None):
                if (user.karma == None):
                    user.karma = 0
                if (reply_to_mark.correct != True):
                    user.karma += reply_to_mark.upvote
            reply_to_mark.correct = True
    else:
        if (status == '0'):
            reply_to_mark.solution = False
            reply_to_mark.correct = False
            user = User.query.filter_by(id=reply_to_mark.author).first()
            if (user != None):
                if (user.karma == None):
                    user.karma = 0
                user.karma -= reply_to_mark.upvote
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt,year=year,questionid=questionid))

#mark reply correct
@login_required
@app.route('/mark_reply_correct/<courseshrt>/<year>/<questionid>/<replyid>/<status>', methods=['POST'])
def mark_reply_correct(courseshrt,year,questionid,replyid,status):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and (status == '0' or status == '1') and is_reply_valid(replyid, Reply, questionid)):
        return bad_request(400)

    if (not (is_course_author(current_user, Course, courseshrt)) and not is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)

    reply_to_mark = Reply.query.filter_by(id=replyid).first()
    if(status == '1'):
        reply_to_mark.correct = True
        user = User.query.filter_by(id=reply_to_mark.author).first()
        if (user != None):
            if (user.karma == None):
                user.karma = 0
            user.karma += reply_to_mark.upvote
    else:
        reply_to_mark.correct = False
        user = User.query.filter_by(id=reply_to_mark.author).first()
        if (user != None):
            if (user.karma == None):
                user.karma = 0
            user.karma -= reply_to_mark.upvote
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt,year=year,questionid=questionid))

#upvote
@app.route('/upvote_reply/<courseshrt>/<year>/<questionid>/<replyid>/<operation>', methods=['GET'])
@login_required
def upvote_reply(replyid, courseshrt,year, questionid, operation):
    if (current_user.is_authenticated):

        if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question) and (operation == 'up' or operation == 'down') and is_reply_valid(replyid, Reply, questionid)):
            return bad_request(400)

        if (not (is_course_member(current_user, Course, courseshrt, course_registration)) and not is_admin(current_user, Role, UserRoles)):
            return unauthorized(401)

        reply_to_upvote = Reply.query.filter_by(id=replyid).first()
        already_upvoted = Upvote.query.filter_by(user=current_user.id, reply=replyid).first()

        if not already_upvoted:
            if operation == 'up':
                up_count = len(Upvote.query.filter_by(user=current_user.id, question=questionid, value=1).all())
                if up_count < 3:
                    reply_to_upvote.upvote += 1
                    new_upvote = Upvote(user=current_user.id, reply=replyid, value=1, question=questionid)
                    db.session.add(new_upvote)
            else:
                down_count = len(Upvote.query.filter_by(user=current_user.id, question=questionid, value=0).all())
                if down_count < 3:
                    reply_to_upvote.upvote -= 1
                    new_upvote = Upvote(user=current_user.id, reply=replyid, value=0, question=questionid)
                    db.session.add(new_upvote)
        else:
            if operation == 'up' and already_upvoted.value == 0:
                up_count = len(Upvote.query.filter_by(user=current_user.id, question=questionid, value=1).all())
                if up_count < 3:
                    reply_to_upvote.upvote += 2
                    already_upvoted.value = 1
            elif operation == 'up' and already_upvoted.value == 1:
                reply_to_upvote.upvote -= 1
                db.session.delete(already_upvoted)
            elif operation == 'down' and already_upvoted.value == 1:
                down_count = len(Upvote.query.filter_by(user=current_user.id, question=questionid, value=0).all())
                if down_count < 3:
                    reply_to_upvote.upvote -= 2
                    already_upvoted.value = 0
            elif operation == 'down' and already_upvoted.value == 0:
                reply_to_upvote.upvote += 1
                db.session.delete(already_upvoted)
        db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt, year=year, questionid=questionid, _anchor=str(replyid)))

# Function to add response to reply
@app.route('/add_response/<courseshrt>/<year>/<questionid>/<reply>', methods=['POST'])
@login_required
def add_response(courseshrt,year,questionid,reply):

    if not (is_question_valid(questionid, Question, courseshrt) and is_course_valid(courseshrt, Course) and is_year_valid(year, questionid, Question)):
        return bad_request(400)
        
    reply_msg = Reply.query.filter_by(id=reply).first()

    if not (is_reply_valid(reply_msg.id, Reply, questionid)):
        return bad_request(400)

    if (request.form['message'] != None):
        if (len(request.form['message']) > 300 or len(request.form['message']) == 0):
            return bad_request(400)

    question = Question.query.get(int(questionid))
    question.lastUpdated = datetime.now()

    if (not (is_course_member(current_user, Course, courseshrt, course_registration)) and not (is_admin(current_user, Role, UserRoles))):
        return unauthorized(401)

    message = request.form['message']
    usr = User.query.get(current_user.id)
    new_response = Response(reply=reply, message=message, date=datetime.now(), author=current_user.id, question=questionid) 
    db.session.add(new_response)

    page_name = "/forum/" + courseshrt + '/' + year + '/' + questionid
    update_last_visit(current_user, Visited, page_name)
    db.session.commit()
    return redirect(url_for('thread',courseshrt=courseshrt, year=year, questionid=questionid, _anchor='response' + str(new_response.id)))

# function to change user role
@app.route('/change_role/<userid>', methods=['GET'])
@login_required
def change_role(userid):

    role = request.args["roles"]
    if (role not in ['student', 'admin', 'mod']):
        return bad_request(400)


    roleid = Role.query.filter_by(name=role).first()
    user = User.query.get(userid)

    if (user == None):
        return page_not_found(404)

    if not (is_admin(current_user, Role, UserRoles)):
        return unauthorized(401)


    # If "student" was selected, this will be none... obviously...
    if (roleid == None):

        # If the student role was given, we need to remove the record from the db
        UserRoles.query.filter_by(user_id=userid).delete()
        db.session.commit()

        curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())
        role_names = get_role_names(Role, UserRoles.query.filter_by(user_id=userid).all())
        return render_template('profile.html', user=user, role_names=role_names, curr_roles=curr_roles)


    new_role_user = UserRoles(user_id=userid, role_id=roleid.id)

    # If the user had no role (was student), give him the new one (If it's also not student)
    # Note: role_names should always have one member (if user is nothing, he is defualt)
    role_names = get_role_names(Role, UserRoles.query.filter_by(user_id=userid).all())
    
    # If the user was assigned the same role he already has, dont do anything
    if (role_names[0].upper() == role.upper()):
        pass

    # If he was given something new, add it to the db
    elif (role != "student"):
        # If he already has a role, remove it first
        if (new_role_user != None):
            UserRoles.query.filter_by(user_id=userid).delete()
            db.session.commit()

        db.session.add(new_role_user)
        db.session.commit()

    # update role names cuz of the assigning that might have gone through in the lines above
    role_names = get_role_names(Role, UserRoles.query.filter_by(user_id=userid).all())
    curr_roles = get_role_names(Role, UserRoles.query.filter_by(user_id=current_user.id).all())
    return render_template('profile.html', user=user, role_names=role_names, curr_roles=curr_roles)

# Function to create page with course best userrs
@app.route('/course_top/<courseshrt>', methods=['GET'])
def course_top(courseshrt):


    test_course = Course.query.filter_by(shortcut=courseshrt).first()
    if (test_course == None):
        return page_not_found(404)

    if (test_course.approved == False):
        if (current_user.is_authenticated):
            if not (is_admin(current_user, Role, UserRoles) or is_mod(current_user, Role, UserRoles) or is_course_author(current_user, Course, courseshrt)):
                return unauthorized(401)
        else:
            return unauthorized(401)

    questions_in_course = Question.query.filter_by(course=courseshrt, solved=True).all()
    curr_year = Years.query.all()
    if (curr_year != []):
        curr_year = curr_year[len(curr_year)-1].year
    else:
        curr_year = 0
    people_karma = {}
    user_ids = []
    for question in questions_in_course:
        q_replies = Reply.query.filter_by(question=question.id).all()
        for reply in q_replies:
            person = User.query.get(reply.author)
            author = None
            if (person == None):
                author = "DELETED USER"
                user_ids.append(-1)
            else:
                author = person.username
                user_ids.append(person.id)
            karma = reply.upvote
            if (author in people_karma):
                prev_karma = people_karma.get(author)
                people_karma[author] = int(karma) + int(prev_karma)
            else:
                people_karma[author] = karma

            if (person == None):
                people_karma[author] = 0

    # https://stackoverflow.com/a/613218/13279982
    people_karma = sorted(people_karma.items(), key=lambda x: x[1], reverse=True)

    people_karma = list(people_karma)[:10] # We want only top 10

    return render_template('course_top.html', courseshrt=courseshrt, karma_list=people_karma, user_ids=user_ids, year=curr_year)

# Function to create page with server-wide top users by their karma
@app.route('/users_top/', methods=['GET'])
def users_top():

    users = User.query.order_by('karma').limit(50).all()

    people_karma = {}

    user_ids = {}

    for user in users:
        if (user.karma == None):
            user.karma = 0
        user_ids[user.id] = user.karma
        people_karma[user.username] = user.karma

    people_karma = sorted(people_karma.items(), key=lambda x: x[1], reverse=True)
    user_ids = sorted(user_ids.items(), key=lambda x: x[1], reverse=True)
    people_karma = list(people_karma) # We want only top 10
    user_ids = list(user_ids)

    return render_template('users_top.html', karma_list=people_karma, users=user_ids)

############################
# ERROR HANDLING FUNCTIONS #
############################
@app.errorhandler(400)
def bad_request(error):
    return render_template('400.html'), 400

@app.errorhandler(401)
def unauthorized(error):
    return render_template('401.html'), 401

@app.errorhandler(404)
def page_not_found(error):
    return render_template('404.html'), 404

@app.errorhandler(405)
def not_allowed_method(error):
    return render_template('405.html'), 405

@app.errorhandler(500)
def internal_error(error):
    return render_template('500.html'), 500
