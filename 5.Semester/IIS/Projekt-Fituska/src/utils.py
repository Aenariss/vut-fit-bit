from datetime import datetime

def get_years(years):
    new_yrs = []
    for year in years:
        tmp = str(year)
        tmp = tmp.split(' ')
        new_yrs.append(int(tmp[1][0:len(tmp[1])-1]))
    new_yrs.reverse()
    return new_yrs

def get_user_names(user_table, results):
    names = []
    for row in results:
        user = user_table.query.filter_by(id=row.user).first()
        if (user != None):
            tmp = [user.name, user.surname]
            names.append(tmp)

    return names

def compare_dates(elements, last_visit):
    mark_new = []
    for element in elements:
        if element.date > last_visit:
            mark_new.append(1)
        else:
            mark_new.append(0)
    return mark_new

def compare_dates_overwrite(elements, last_visit, array, index):
    for element in elements:
        if element.date > last_visit:
            array[index] = 1
    return array

def get_authors(User_table, courses):
    author_username = []
    author_id = []
    for course in courses:
        usr = User_table.query.get(course.author)
        # If user was deleted
        id = None
        username = None
        if (usr == None):
            username = "DELETED USER"
            id = -1
        else:
            username = usr.username
            id = usr.id
        author_username.append(username)
        author_id.append(id)
    return author_username, author_id

def get_authors_role_karma(User_table, Role_table, UserRoles_table, courses):
    author_role = []
    author_karma = []
    for course in courses:
        usr = User_table.query.get(course.author)
        role = None
        karma = None
        if(usr == None):
            role = ["DELETED USER"]
            karma = 0
        else:
            role = get_role_names(Role_table, UserRoles_table.query.filter_by(user_id=usr.id).all())
            karma = usr.karma
        author_role.append(role)
        author_karma.append(karma)
    return author_role, author_karma                

def get_last_visit(current_user, visited_table, pagename):
    last_visit = '01/01/1970 00:00:00'  # default hodnota, proc ne treba tahle (staci jakakoliv v minulosti)
    last_visit = datetime.strptime(last_visit, '%d/%m/%Y %H:%M:%S')

    if (current_user.is_authenticated):
        prev_visit = visited_table.query.filter(visited_table.user==current_user.id, visited_table.page==pagename).first()
        if (prev_visit != None):
            last_visit = prev_visit.visited
    return last_visit

def get_last_visit_with_update(visited_table, current_user, pagename, db):
    last_visit = '01/01/1970 00:00:00'  # default hodnota, proc ne treba tahle (staci jakakoliv v minulosti)
    last_visit = datetime.strptime(last_visit, '%d/%m/%Y %H:%M:%S')

    if (current_user.is_authenticated):
        prev_visit = visited_table.query.filter(visited_table.user==current_user.id, visited_table.page==pagename).first()
        if (prev_visit != None):
            last_visit = prev_visit.visited
            prev_visit = visited_table.query.filter(visited_table.user==current_user.id, visited_table.page==pagename).update({"visited": datetime.now()})
            db.session.commit()
        else:
            visiting = visited_table(page=pagename, user=current_user.id, visited=datetime.now())
            db.session.add(visiting)
            db.session.commit()

    return last_visit

def update_last_visit(current_user, visited_table, pagename):    # Tahle funkce musi byt nasledovana zavolanim db.commit(), jinak nebude fungovat!
    visited_table.query.filter(visited_table.user==current_user.id, visited_table.page==pagename).update({"visited": datetime.now()})

# Get role names to print in user profile and use in checking
def get_role_names(role_table, roles):
    names = []
    if (roles == []):
        names.append("student")
        return names

    roles = role_table.query.filter_by(id=roles[0].role_id).all()
    for role in roles:
        names.append(role.name)
    # This should probably never happen, a remain of a previous function... Better keep it here anyway tho
    if (len(names) == 0):
        names.append("student")
    return names

def get_unread_questions(current_user, question_table, reply_table, response_table, visited_table, courseshrt, year, last_visit):
    questions = question_table.query.filter_by(course=courseshrt, year=year).all()
    new_questions = compare_dates(questions, last_visit)

    # Funkcionalita "Novych" prispevku zvyzuje narocnost celeho procesu - uzivatel to nepozna (budou to rady maximalne desetin vteriny)
    i = 0
    for question in questions:
        replies = reply_table.query.filter_by(question=question.id).all()
        responses = response_table.query.filter_by(question=question.id).all()
        tmp_path = "/forum/" + str(courseshrt) + '/' + str(year) + '/' + str(question.id)
        tmp_last_visit = get_last_visit(current_user, visited_table, tmp_path)
        new_questions = compare_dates_overwrite(replies, tmp_last_visit, new_questions, i)
        new_questions = compare_dates_overwrite(responses, tmp_last_visit, new_questions, i)
        i -=- 1

    return new_questions

def get_unread_courses(current_user, db, courses, question_table, reply_table, response_table, visited_table, pagename, new_yrs, status=0):

    last_visit = None
    if (status == 1):
        last_visit = get_last_visit(current_user, visited_table, pagename)
    else:
        last_visit = get_last_visit_with_update(visited_table, current_user, pagename, db)
    new_courses = compare_dates(courses, last_visit)
    unread_years = []
    k = 0
    for course in courses:
        course_flag = 0
        tmp = []
        for year in new_yrs:
            page_name = "/forum/" + course.shortcut + '/' + str(year)
            last_visit = get_last_visit(current_user, visited_table, page_name)
            new_questions = get_unread_questions(current_user, question_table, reply_table, response_table, visited_table, course.shortcut, year, last_visit)
            flag = 0
            for q_update in new_questions:
                if q_update == 1:
                    flag = 1
                    course_flag = 1
            tmp.append(flag)
        unread_years.append(tmp)
        new_courses[k] = course_flag
        k -=- 1
    return new_courses, unread_years

def get_course_shortcuts(questions, course_table):
    shortcuts = []
    courses = []
    for question in questions:
        course = course_table.query.filter_by(shortcut=question.course).first()
        if (course != [] and course != None):
            courses.append(course)
            shortcuts.append([course.shortcut, course.name])
        else:
            print("This should NOT happen, something went wrong somewhere")
    
    return shortcuts, courses

def is_course_author(current_user, course_table, courseshrt):
    if current_user.is_authenticated:
        course = course_table.query.filter_by(shortcut=courseshrt).first()
        if course.author != current_user.id:
            return False
        else:
            return True
    else:
        return False

def is_course_member(current_user, course_table, courseshrt, course_reg_table):
    if current_user.is_authenticated:
        course = course_table.query.filter_by(shortcut=courseshrt).first()
        if not (is_course_author(current_user, course_table, courseshrt)):
            authorization = course_reg_table.query.filter_by(course=course.shortcut, user=current_user.id).first()
            if (authorization == None):
                return False
            if not (authorization.approved):
                return False
            else:
                return True
        else:
            return True
    else:
        return False

def is_admin(current_user, role_table, roles_users_table):
    if current_user.is_authenticated:
        curr_roles = get_role_names(role_table, roles_users_table.query.filter_by(user_id=current_user.id).all())
        if not ("admin" in curr_roles):
            return False
        else:
            return True
    else:
        return False
        
def is_mod(current_user, role_table, roles_users_table):
    if current_user.is_authenticated:
        curr_roles = get_role_names(role_table, roles_users_table.query.filter_by(user_id=current_user.id).all())
        if not ("mod" in curr_roles):
            return False
        else:
            return True
    else:
        return False

def is_author(current_user, thing):
    if current_user.is_authenticated:
        if thing.author != current_user.id:
            return False
        else:
            return True
    else:
        return False

def get_reply_author_info(replies, current_user, user_table, role_table, user_roles_table, upvote_table):
    rep_author_username = []
    rep_author_role = []
    rep_author_karma = []
    rep_ids = []
    author_ids = []
    rep_upvotes = []
    for reply in replies:
        author = user_table.query.get(reply.author)
        if (current_user.is_authenticated):
            upvote_status = upvote_table.query.filter_by(user=current_user.id, reply=reply.id).first()
            if (upvote_status == None):
                rep_upvotes.append(0)
            else:
                if (upvote_status.value == 1):
                    rep_upvotes.append(1)
                else:
                    rep_upvotes.append(2)
        else:
            rep_upvotes.append(0)

        if (author == None):
            rep_author_username.append("DELETED USER")
            rep_author_role.append(["student"])
            rep_author_karma.append(0)
            author_ids.append(-1)
        else:
            rep_author_username.append(author.username)
            rep_author_role.append(get_role_names(role_table, user_roles_table.query.filter_by(user_id=author.id).all()))
            rep_author_karma.append(author.karma)
            author_ids.append(author.id)
        rep_ids.append(reply.id)

    return rep_author_username, rep_author_role, rep_author_karma, rep_ids, rep_upvotes, author_ids

def is_question_valid(questionid, question_table, courseshrt):
    que = question_table.query.filter_by(id=questionid).first()
    if que == None:
        return False
    else:
        if str(que.course) == str(courseshrt):
            return True
        else:
            return False

def is_course_valid(shortcut, course_table):
    crs = course_table.query.filter_by(shortcut=shortcut).first()
    if crs == None:
        return False
    else:
        return True

def is_year_valid(year, questionid, q_table):
    question = q_table.query.filter_by(id=questionid).first()
    if (str(question.year) == str(year)):
        return True
    else:
        return False

def is_reply_valid(replyid, reply_table, questionid):
    reply = reply_table.query.filter_by(id=replyid).first()
    if reply == None:
        return False
    else:
        if str(reply.question) == str(questionid):
            return True
        else:
            return False

def is_response_valid(respid, Response, replyid):
    response = Response.query.filter_by(id=respid).first()

    if (response == None):
        return False
    else:
        if str(response.reply) == str(replyid):
            return True
        else:
            return False

def is_course_in_grade(courseid, coursegrade, Course):
    course = Course.query.filter_by(shortcut=courseid).first()
    if course == None:
        return False
    else:
        if str(course.grade) == str(coursegrade):
            return True
        else:
            return False

def get_approved_questions(userid, Question, Course):
    check_questions = Question.query.filter_by(author=userid).all()
    questions = []
    for question in check_questions:
        question_course = Course.query.filter_by(shortcut=question.course).first()
        if (question_course != None):
            if (question_course.approved == True):
                questions.append(question)
    return questions