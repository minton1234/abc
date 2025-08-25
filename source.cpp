BOOL CMFCApplication8Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // �V�X�e�����j���[�Ɂu�o�[�W�������...�v��ǉ�
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        CString strAboutMenu;
        if (strAboutMenu.LoadString(IDS_ABOUTBOX) && !strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // --- ���j�^������ ---
    m_hMonitor = MonitorFromWindow(GetSafeHwnd(), MONITOR_DEFAULTTOPRIMARY);

    DWORD count = 0;
    if (GetNumberOfPhysicalMonitorsFromHMONITOR(m_hMonitor, &count) && count > 0) {
        if (GetPhysicalMonitorsFromHMONITOR(m_hMonitor, 1, &m_PhysicalMonitor)) {

            // �X���C�_�[�����W�͏�� 0..100
            m_slider.SetRange(0, 100, TRUE);
            m_slider.SetTicFreq(10);

            DWORD minB = 0, curB = 0, maxB = 100;
            if (GetMonitorBrightness(m_PhysicalMonitor.hPhysicalMonitor, &minB, &curB, &maxB) &&
                maxB > minB)
            {
                // ���������W[minB..maxB] �� UI�����W[0..100]�֐��K��
                int pos = (int)(((curB - minB) * 100.0 / (maxB - minB)) + 0.5);
                if (pos < 0) pos = 0;
                if (pos > 100) pos = 100;
                m_slider.SetPos(pos);
            }
            else {
                m_slider.SetPos(50); // ���s���͒���
            }
        }
    }
    else {
        // ���j�^�擾���s���� 0..100 �Œ�
        m_slider.SetRange(0, 100, TRUE);
        m_slider.SetTicFreq(10);
        m_slider.SetPos(50);
    }

    return TRUE;
}

void CMFCApplication8Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar->GetSafeHwnd() == m_slider.GetSafeHwnd()) {

        DWORD minB = 0, curB = 0, maxB = 100;
        if (m_PhysicalMonitor.hPhysicalMonitor &&
            GetMonitorBrightness(m_PhysicalMonitor.hPhysicalMonitor, &minB, &curB, &maxB) &&
            maxB > minB)
        {
            int uiPos = m_slider.GetPos();
            if (uiPos < 0) uiPos = 0;
            if (uiPos > 100) uiPos = 100;

            // UI 100% �� ���� maxB, UI 0% �� ���� minB
            DWORD newB = minB + (DWORD)((uiPos * (double)(maxB - minB) / 100.0) + 0.5);
            if (uiPos == 100) newB = maxB;  // �ۂߌ덷�΍�
            if (uiPos == 0)   newB = minB;

            SetMonitorBrightness(m_PhysicalMonitor.hPhysicalMonitor, newB);
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



